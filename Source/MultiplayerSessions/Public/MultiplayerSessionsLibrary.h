#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MultiplayerSessionsLibrary.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Online|AdvancedSessions|Seamless", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool ServerTravel(UObject* WorldContextObject, const FString& InURL, bool bAbsolute, bool bShouldSkipGameNotify);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Online|AdvancedSessions|Seamless", meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject"))
	static bool ServerTravelToLevel(UObject* WorldContextObject, const TSoftObjectPtr<UWorld> LobbyServerTravelMap, bool bAbsolute, bool bShouldSkipGameNotify);

	static FString GetServerTravelLobbyUrl(const TSoftObjectPtr<UWorld>& LobbyServerTravelMap);
};
