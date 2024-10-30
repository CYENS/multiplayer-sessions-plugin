// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

class UMultiplayerSessionsSubsystem;
class UButton;

DECLARE_LOG_CATEGORY_EXTERN(LogMultiplayerSessionsMenu, Log, All);

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(
		const int32 NumberPublicConnections = 4,
		const FString TypeOfMatch = TEXT("FreeForAll"),
		const TSoftObjectPtr<UWorld> LobbyMap = nullptr,
		const TSoftObjectPtr<UWorld> SessionMap = nullptr
	);
	TSoftObjectPtr<UWorld> LobbyMapAsset;
	TSoftObjectPtr<UWorld> SessionMapAsset;

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	// callbacks for the custom delegates on the MultiplayerSessionsSubsystem
	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, FString SessionId, bool bWasSuccessful);
	FString GetServerTravelLobbyMapPath() const;
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void OnJoinSessionComplete(const FName& SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(bool bWasSuccessful);
	FString GetServerTravelSessionMapPath() const;
	void OnDestroySessionComplete(bool bWasSuccessful);
	
	// menu setup functions
	bool TryBindCallbacksToMultiplayerSessionsSubsystem();
	bool TrySetMultiplayerSessionsSubsystem();
	bool TryFocusWidgetAndShowMouse();

private:
	// with BindWidget, we can bind the UButton* to the actual button in the UMG !as long as the name matches!
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UFUNCTION()
	void HostButtonClicked();
	
	UFUNCTION()
	void JoinButtonClicked();
	
	UFUNCTION(BlueprintCallable, Category="MultiplayerSessions")
	void StartMultiplayerSession() const;
	
	void DisableJoinAndHostButtons();

	void MenuTeardown();

	int32 NumPublicConnections { 4 };
	FString MatchType { "FreeForAll" };
};
