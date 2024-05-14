// Fill out your copyright notice in the Description page of Project Settings.

#include "Menu.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

DEFINE_LOG_CATEGORY(LogMultiplayerSessionsMenu);

void UMenu::MenuSetup(
	const int32 NumberPublicConnections,
	const FString TypeOfMatch,
	const TSoftObjectPtr<UWorld> LobbyMap,
	const TSoftObjectPtr<UWorld> SessionMap
)
{
	NumPublicConnections = NumberPublicConnections;
	MatchType = TypeOfMatch;
	LobbyMapAsset = LobbyMap;
	SessionMapAsset = SessionMap;
	
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	SetIsFocusable(true);

	if(!TryFocusWidgetAndShowMouse())
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Could not focus widget and show mouse"));
	}

	if(!TrySetMultiplayerSessionsSubsystem())
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Could not set MultiplayerSessionsSubsystem"));
	}
	else
	{
		MultiplayerSessionsSubsystem;
	}
	
	if(!TryBindCallbacksToMultiplayerSessionsSubsystem())
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Failed to bind callbacks to MultiplayerSessionsSubsystem"));
	}
}

bool UMenu::TryFocusWidgetAndShowMouse()
{
	bool bHasSuccessfullyFocusedWidgetAndShownMouse = false;
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->SetShowMouseCursor(true);
			bHasSuccessfullyFocusedWidgetAndShownMouse = true;
		}
		else
		{
			UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: PlayerController is null"));
		}
	}
	else
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: World is null"));
	}
	return bHasSuccessfullyFocusedWidgetAndShownMouse;
}


bool UMenu::TrySetMultiplayerSessionsSubsystem()
{
	bool bHasSuccessfullySetMultiplayerSessionsSubsystem = false;
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		if(const UGameInstance* GameInstance = GetGameInstance())
		{
			MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
			if (MultiplayerSessionsSubsystem)
			{
				bHasSuccessfullySetMultiplayerSessionsSubsystem = true;
			}
			else
			{
				UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: MultiplayerSessionsSubsystem is null"));
			}
		}
		else
		{
			UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: GameInstance is null, could not get MultiplayerSessionsSubsystem"));
		}
	}

	return bHasSuccessfullySetMultiplayerSessionsSubsystem;
}

bool UMenu::TryBindCallbacksToMultiplayerSessionsSubsystem()
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return false;
	}
	
	MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUObject(this, &ThisClass::OnCreateSessionComplete);
	MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsComplete);
	MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
	// MultiplayerSessionsSubsystem->MultiplayerOnStartSessionComplete.AddUObject(this, &ThisClass::OnStartSessionComplete);
	MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddUObject(this, &ThisClass::OnDestroySessionComplete);
	return true;
}

bool UMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (!HostButton || !JoinButton)
	{
		return false;
	}
	
	HostButton->OnClicked.AddDynamic(this, &UMenu::HostButtonClicked);
	JoinButton->OnClicked.AddDynamic(this, &UMenu::JoinButtonClicked);
	return true;
}

void UMenu::HostButtonClicked()
{
	DisableJoinAndHostButtons();
	
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->CreateSession(NumPublicConnections, MatchType, NAME_GameSession, TMap<FName, FString> ());
	}
}

void UMenu::JoinButtonClicked()
{
	DisableJoinAndHostButtons();
	
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void UMenu::StartMultiplayerSession() const
{
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->StartSession();
	}
	else
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: MultiplayerSessionsSubsystem is null"));
	}
}

void UMenu::DisableJoinAndHostButtons()
{
	HostButton->SetIsEnabled(false);
	JoinButton->SetIsEnabled(false);
}

void UMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Failed to create session"));
		HostButton->SetIsEnabled(true);
	}
	UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: Session Created successfully"));

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Failed to get World, cannot server travel to Lobby"));
		HostButton->SetIsEnabled(true);
		return;
	}
	
	const FString ServerTravelLobbyMapPath = GetServerTravelLobbyMapPath();
	UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: ServerTravelLobbyMapPath set to: %s"), *ServerTravelLobbyMapPath);
	
	if (World->ServerTravel(ServerTravelLobbyMapPath))
	{
		UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: Listen Server Travelled to LobbyMap"));
	}
	else
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Listen Server Failed to travel to LobbyMap"));
		HostButton->SetIsEnabled(true);
	}
}

FString UMenu::GetServerTravelLobbyMapPath() const
{
	FString ServerTravelLobbyMapPath;
	if (!LobbyMapAsset.IsNull())
	{
		// CanonicalAssetPath is something like "/Game/ThirdPerson/Maps/LobbyMap.LobbyMap"
		const FString CanonicalAssetPath = LobbyMapAsset.ToString();
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

void UMenu::OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MultiplayerSessionsSubsystem is null"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MultiplayerSessionsSubsystem is null"));
		}
		JoinButton->SetIsEnabled(true);
		return;
	}

	bool SuccessfullyFoundSessionToJoin = false;
	if (bWasSuccessful)
	{
		for (const FOnlineSessionSearchResult& SearchResult : SearchResults)
		{
			FString Id = SearchResult.GetSessionIdStr();
			FString Name = SearchResult.Session.OwningUserName;
			FString RetrievedMatchType {""};
			SearchResult.Session.SessionSettings.Get(FName("MatchType"), RetrievedMatchType);
			FString SecretKeyValue {""};
			SearchResult.Session.SessionSettings.Get(FName("SecretKey"), SecretKeyValue);
			UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: Session found | Id: %s | Name: %s | MatchType %s | SecretKeyValue: %s |"), *Id, *Name, *MatchType, *SecretKeyValue);
			if (SecretKeyValue == FString("PREMIERE"))
			{
				UE_LOG(LogTemp, Log, TEXT("Joining session: %s"), *Id);
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Joining session: %s"), *Id));
				}
				MultiplayerSessionsSubsystem->JoinSession(SearchResult);
				SuccessfullyFoundSessionToJoin = true;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find sessions"));
		SuccessfullyFoundSessionToJoin = false;
	}
	JoinButton->SetIsEnabled(!SuccessfullyFoundSessionToJoin);
	HostButton->SetIsEnabled(!SuccessfullyFoundSessionToJoin);
}

void UMenu::OnJoinSessionComplete(const FName& SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MultiplayerSessionsSubsystem is null"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("MultiplayerSessionsSubsystem is null"));
		}
		JoinButton->SetIsEnabled(true);
		HostButton->SetIsEnabled(true);
		return;
	}

	FString Address;
	if (!MultiplayerSessionsSubsystem->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get resolved connect string"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to get resolved connect string"));
		}
		JoinButton->SetIsEnabled(true);
		HostButton->SetIsEnabled(true);
		return;
	}
	// print Address to the screen
	UE_LOG(LogTemp, Log, TEXT("Resolved connect string: %s"), *Address);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Resolved connect string: %s"), *Address));
	}

	if(!MultiplayerSessionsSubsystem->TryFirstLocalPlayerControllerClientTravel(Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to travel to session"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to travel to session"));
		}
		JoinButton->SetIsEnabled(true);
		HostButton->SetIsEnabled(true);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Travel to session successful"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Travel to session successful"));
		}
	}
}

void UMenu::OnStartSessionComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Failed to start session"));
		return;
	}
	UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: Session started successfully"));

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Failed to get World, cannot server travel to session map"));
		return;
	}
	
	const FString ServerTravelLobbyMapPath = GetServerTravelSessionMapPath();
	UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: ServerTravelSessionMapPath set to: %s"), *ServerTravelLobbyMapPath);
	
	if (World->ServerTravel(ServerTravelLobbyMapPath))
	{
		UE_LOG(LogMultiplayerSessionsMenu, Log, TEXT("Menu: Listen Server Travelled to %s"), *ServerTravelLobbyMapPath);
	}
	else
	{
		UE_LOG(LogMultiplayerSessionsMenu, Error, TEXT("Menu: Listen Server Failed to travel to session map %s"), *ServerTravelLobbyMapPath);
	}
}

FString UMenu::GetServerTravelSessionMapPath() const
{
	FString ServerTravelSessionMapPath;
	if (!LobbyMapAsset.IsNull())
	{
		// CanonicalAssetPath is something like "/Game/ThirdPerson/Maps/LobbyMap.LobbyMap"
		const FString CanonicalAssetPath = LobbyMapAsset.ToString();
		// Remove the ".*" part and add "?listen" to the end
		if (
			const int32 DotPosition = CanonicalAssetPath.Find(".");
			DotPosition != INDEX_NONE
		)
		{
			ServerTravelSessionMapPath = CanonicalAssetPath.Left(DotPosition) + "?listen";
		}
	}
	else
	{
		const FString DefaultSessionsMapPath { "/MultiplayerSessions/Maps/LVL_MainLevelMultiplayerSessionsDemo?listen" };
		ServerTravelSessionMapPath = DefaultSessionsMapPath;
	}
	return ServerTravelSessionMapPath;
}

void UMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
}

void UMenu::NativeDestruct()
{
	MenuTeardown();
	
	Super::NativeDestruct();
}

void UMenu::MenuTeardown()
{
	RemoveFromParent();
	if (const UWorld* World = GetWorld())
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			PlayerController->SetInputMode( FInputModeGameOnly {} );
			PlayerController->SetShowMouseCursor(false);
		}
	}
}
