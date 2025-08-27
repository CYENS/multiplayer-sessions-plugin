#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "AutoSessionPlayerController.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API AAutoSessionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Players using the same key will meet in the same session (e.g., "EU-DUOS-42")
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AutoSession")
	FString SessionKey = TEXT("DEFAULT_KEY");

	// Map the host opens after the session is created (must be included in packaging)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AutoSession")
	TSoftObjectPtr<UWorld> LevelToTravel;

	// Public slots when we create a session
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AutoSession")
	int32 MaxPlayers = 16;

	virtual void BeginPlay() override;

private:
	// Online Subsystem
	IOnlineSessionPtr SessionInterface;
	IOnlineIdentityPtr IdentityInterface;

	// Delegates
	FDelegateHandle OnFindSessionsCompleteHandle;
	FDelegateHandle OnCreateSessionCompleteHandle;
	FDelegateHandle OnJoinSessionCompleteHandle;
	FDelegateHandle OnLoginCompleteHandle;

	// Search handle
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// Flow
	void StartAfterLogin();
	void FindSessions();
	void CreateSession();
	void JoinFoundSession(const FOnlineSessionSearchResult& Result);

	// Handlers
	void HandleLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
	void HandleFindSessionsComplete(bool bWasSuccessful);
	void HandleCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void HandleJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	FString GetServerTravelLobbyMapPath() const;
};
