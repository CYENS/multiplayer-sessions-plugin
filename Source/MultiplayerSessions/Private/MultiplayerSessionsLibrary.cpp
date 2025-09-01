
#include "MultiplayerSessionsLibrary.h"

bool UMultiplayerSessionsLibrary::ServerTravel(UObject* WorldContextObject, const FString& FURL, const bool bAbsolute, const bool bShouldSkipGameNotify)
{
	if (!WorldContextObject)
	{
		return false;
	}

	UWorld* const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return false;
	}
	
	return World->ServerTravel(FURL, bAbsolute, bShouldSkipGameNotify);
}

bool UMultiplayerSessionsLibrary::ServerTravelToLevel(UObject* WorldContextObject, const TSoftObjectPtr<UWorld> LobbyServerTravelMap, const bool bAbsolute, const bool bShouldSkipGameNotify)
{
	if (LobbyServerTravelMap.IsNull())
	{
		return false;
	}
	
	const FString Url =  GetServerTravelLobbyUrl(LobbyServerTravelMap);
	return ServerTravel(WorldContextObject, Url, bAbsolute, bShouldSkipGameNotify);
}

FString UMultiplayerSessionsLibrary::GetServerTravelLobbyUrl(const TSoftObjectPtr<UWorld>& LobbyServerTravelMap)
{
	FString ServerTravelLobbyMapPath;
	if (!LobbyServerTravelMap.IsNull())
	{
		// CanonicalAssetPath is something like "/Game/ThirdPerson/Maps/LobbyMap.LobbyMap"
		const FString CanonicalAssetPath = LobbyServerTravelMap.ToString();
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

