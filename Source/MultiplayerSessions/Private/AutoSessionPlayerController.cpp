#include "AutoSessionPlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"
#include "Online/OnlineSessionNames.h"

static const FName SETTING_SESSION_KEY(TEXT("SESSION_KEY"));
static const FName DEFAULT_SESSION_NAME(NAME_GameSession);

void AAutoSessionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Get Online Subsystem interfaces (EOS)
	if (const IOnlineSubsystem* OSS = IOnlineSubsystem::Get())
	{
		SessionInterface  = OSS->GetSessionInterface();
		IdentityInterface = OSS->GetIdentityInterface();
	}

	if (!SessionInterface.IsValid() || !IdentityInterface.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem EOS not available. Check DefaultEngine.ini."));
		return;
	}

	// ------------------ DEV AUTH LOGIN ------------------
	// Preferred: let UE read -AUTH_LOGIN / -AUTH_PASSWORD / -AUTH_TYPE=developer
	//            and call AutoLogin(0).
	OnLoginCompleteHandle = IdentityInterface->AddOnLoginCompleteDelegate_Handle(
		0,
		FOnLoginCompleteDelegate::CreateUObject(this, &AAutoSessionPlayerController::HandleLoginComplete)
	);

	if (!IdentityInterface->AutoLogin(0))
	{
		// Fallback: build credentials directly from the command line or defaults.
		// DevAuth requires a DevAuth tool running locally or remotely.
		// Type: "developer"
		// Id:   "host:port"     (e.g., "127.0.0.1:6300")
		// Token:"DevUserName"   (user configured in the DevAuth tool)
		FString HostPort = TEXT("127.0.0.1:6300");
		FString DevUser  = TEXT("DevUser");

		FParse::Value(FCommandLine::Get(), TEXT("AUTH_LOGIN="), HostPort);   // overrides if provided
		FParse::Value(FCommandLine::Get(), TEXT("AUTH_PASSWORD="), DevUser); // overrides if provided

		FOnlineAccountCredentials Creds;
		Creds.Type  = TEXT("developer");
		Creds.Id    = HostPort;
		Creds.Token = DevUser;

		IdentityInterface->Login(0, Creds);
	}
	// ----------------------------------------------------
}

void AAutoSessionPlayerController::HandleLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	IdentityInterface->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteHandle);

	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("EOS DevAuth login failed: %s"), *Error);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("EOS DevAuth login OK: %s"), *UserId.ToString());
	StartAfterLogin();
}

void AAutoSessionPlayerController::StartAfterLogin()
{
	// Optional: allow -SessionKey=XYZ on the command line
	FString CmdKey;
	if (FParse::Value(FCommandLine::Get(), TEXT("SessionKey="), CmdKey))
	{
		SessionKey = CmdKey;
	}

	FindSessions();
}

void AAutoSessionPlayerController::FindSessions()
{
	if (!SessionInterface.IsValid())
		return;

	SessionSearch = MakeShared<FOnlineSessionSearch>();
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
	SessionSearch->QuerySettings.Set(SETTING_SESSION_KEY, SessionKey, EOnlineComparisonOp::Equals);
	
	OnFindSessionsCompleteHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &AAutoSessionPlayerController::HandleFindSessionsComplete)
	);

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void AAutoSessionPlayerController::HandleFindSessionsComplete(bool bWasSuccessful)
{
	if (!SessionInterface.IsValid())
		return;

	SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteHandle);

	if (bWasSuccessful && SessionSearch.IsValid())
	{
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			if (FString FoundKey;
				Result.Session.SessionSettings.Get(SETTING_SESSION_KEY, FoundKey) && FoundKey == SessionKey
			)
			{
				JoinFoundSession(Result);
				return;
			}
		}
	}

	// None found → host
	CreateSession();
}

void AAutoSessionPlayerController::CreateSession()
{
	if (!SessionInterface.IsValid())
		return;

	// Clean up old session (PIE convenience)
	if (SessionInterface->GetNamedSession(DEFAULT_SESSION_NAME))
	{
		SessionInterface->DestroySession(DEFAULT_SESSION_NAME);
	}

	FOnlineSessionSettings Settings;

	// Mirror your BP checkboxes
	Settings.NumPublicConnections  = MaxPlayers;
	Settings.NumPrivateConnections = 0;

	Settings.bIsLANMatch                         = false; // EOS is online
	Settings.bAllowInvites                       = false;
	Settings.bIsDedicated                        = false;

	Settings.bUsesPresence                       = true;
	Settings.bAllowJoinInProgress                = true;
	Settings.bAllowJoinViaPresence               = true;
	Settings.bShouldAdvertise                    = true;
	Settings.bUseLobbiesIfAvailable              = true;
	Settings.bUseLobbiesVoiceChatIfAvailable     = true;
	Settings.bAntiCheatProtected                 = false;
	Settings.bUsesStats                          = false;

	Settings.Set(SEARCH_PRESENCE, true, EOnlineDataAdvertisementType::ViaOnlineService);
	Settings.Set(SETTING_SESSION_KEY, SessionKey, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	OnCreateSessionCompleteHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &AAutoSessionPlayerController::HandleCreateSessionComplete));

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), DEFAULT_SESSION_NAME, Settings);
}

void AAutoSessionPlayerController::HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!SessionInterface.IsValid())
		return;

	SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteHandle);

	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateSession failed."));
		return;
	}

	// We are the listen host; open the map with ?listen (ServerTravel)
	if (HasAuthority())
	{
		if (UWorld* World = GetWorld())
		{
			const FString Map = GetServerTravelLobbyMapPath();
			UE_LOG(LogTemp, Log, TEXT("Host traveling to %s?listen"), *Map);
			World->ServerTravel(Map, true);
		}
	}
	else
	{
		FString ConnectStr;
		if (SessionInterface->GetResolvedConnectString(SessionName, ConnectStr))
		{
			ClientTravel(ConnectStr, TRAVEL_Absolute);
		}
	}
}

void AAutoSessionPlayerController::JoinFoundSession(const FOnlineSessionSearchResult& Result)
{
	if (!SessionInterface.IsValid())
		return;

	OnJoinSessionCompleteHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &AAutoSessionPlayerController::HandleJoinSessionComplete)
	);

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), DEFAULT_SESSION_NAME, Result);
}

void AAutoSessionPlayerController::HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())
		return;

	SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteHandle);

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("JoinSession failed (%d) — creating new."), (int32)Result);
		CreateSession();
		return;
	}

	FString ConnectStr;
	if (SessionInterface->GetResolvedConnectString(SessionName, ConnectStr))
	{
		UE_LOG(LogTemp, Log, TEXT("Joining %s"), *ConnectStr);
		ClientTravel(ConnectStr, TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Could not resolve connect string — creating instead."));
		CreateSession();
	}
}

FString AAutoSessionPlayerController::GetServerTravelLobbyMapPath() const
{
	FString ServerTravelLobbyMapPath;
	if (!LevelToTravel.IsNull())
	{
		// CanonicalAssetPath is something like "/Game/ThirdPerson/Maps/LobbyMap.LobbyMap"
		const FString CanonicalAssetPath = LevelToTravel.ToString();
		// Remove the ".*" part and add "?listen" to the end
		if (
			const int32 DotPosition = CanonicalAssetPath.Find(".");
			DotPosition != INDEX_NONE
		)
		{
			ServerTravelLobbyMapPath = CanonicalAssetPath.Left(DotPosition) + "?listen";
		}
	}
	else
	{
		const FString DefaultLobbyMapPath { "/Game/ThirdPerson/Maps/LobbyMap?listen" };
		ServerTravelLobbyMapPath = DefaultLobbyMapPath;
	}
	return ServerTravelLobbyMapPath;
}
