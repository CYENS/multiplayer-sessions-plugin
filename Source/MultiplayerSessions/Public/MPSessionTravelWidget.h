// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MPSessionTravelWidget.generated.h"

struct FMPSessionSettings;
DECLARE_LOG_CATEGORY_EXTERN(LogMPSessionTravelWidget, Log, All);

class UMultiplayerSessionsSubsystem;

struct FBPSessionResult;

UCLASS()
class MULTIPLAYERSESSIONS_API UMPSessionTravelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

	TSoftObjectPtr<UWorld> LobbyMapAsset;
	TSoftObjectPtr<UWorld> SessionMapAsset;
	
	UFUNCTION(BlueprintCallable, Category="Multiplayer Sessions")
	void CreateSession(
		const TSoftObjectPtr<UWorld> LobbyServerTravelMap,
		const FMPSessionSettings& SessionSettings,
		const TMap<FName, FString>& ExtraSessionSettings
	);

	UFUNCTION(BlueprintCallable, Category="Μultiplayer Sessions")
	void FindSessions(const int32 MaxSearchResults = 1000) const;
	UFUNCTION(BlueprintCallable, Category="Μultiplayer Sessions")
	void JoinSession(const FBPSessionResult& SearchResult);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Multiplayer Sessions")
	void OnSessionCreated(const FName SessionName, const FString& SessionId, const bool bWasSuccessful);
	
	UFUNCTION(BlueprintImplementableEvent, Category="Multiplayer Sessions")
	void OnSessionsFound(const TArray<FBPSessionResult>& SearchResults, const bool bWasSuccessful);

protected:
	virtual void NativeDestruct() override;

	void OnCreateSessionComplete(FName SessionName, FString SessionId, bool bWasSuccessful);
	void OnFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void OnJoinSessionComplete(const FName& SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(bool bWasSuccessful);
	
	FString GetServerTravelLobbyMapPath() const;
	FString GetServerTravelSessionMapPath() const;
	
	// menu setup functions
	bool TryBindCallbacksToMultiplayerSessionsSubsystem();
	bool TrySetMultiplayerSessionsSubsystem();
	bool TryFocusWidgetAndShowMouse();

private:
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	UFUNCTION(BlueprintCallable, Category="MultiplayerSessions")
	void StartMultiplayerSession() const;

	void MenuTeardown();
};
