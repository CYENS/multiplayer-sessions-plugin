// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerSessionsSearchResult.h"
#include "Components/ActorComponent.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionsComponent.generated.h"

enum class EJoinSessionResult : uint8;

class UMultiplayerSessionsSubsystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlueprintCreateSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlueprintFindSessionsComplete, const TArray<FMultiplayerSessionsSearchResult>, SearchResults, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBlueprintJoinSessionComplete, const FName&, SessionName, EJoinSessionResult, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlueprintStartSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlueprintDestroySessionComplete, bool, bWasSuccessful);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMultiplayerSessionsComponent();
	
	/** Initialize and bind to the subsystem events */
	virtual void InitializeComponent() override;

	// Blueprint Assignment events for session management
	UPROPERTY(BlueprintAssignable, Category = "Multiplayer Sessions Events")
	FOnBlueprintCreateSessionComplete OnCreateSessionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Multiplayer Sessions Events")
	FOnBlueprintFindSessionsComplete OnFindSessionsComplete;

	UPROPERTY(BlueprintAssignable, Category = "Multiplayer Sessions Events")
	FOnBlueprintJoinSessionComplete OnJoinSessionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Multiplayer Sessions Events")
	FOnBlueprintStartSessionComplete OnStartSessionComplete;

	UPROPERTY(BlueprintAssignable, Category = "Multiplayer Sessions Events")
	FOnBlueprintDestroySessionComplete OnDestroySessionComplete;

	// Blueprint Implementable Events to be overridable in the components blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Sessions Events")
	void OnCreateSession(bool bWasSuccessful);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Sessions Events")
	void OnFindSessions(const TArray<FMultiplayerSessionsSearchResult>& SearchResults, bool bWasSuccessful);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Sessions Events")
	void OnJoinSession(FName SessionName, EJoinSessionResult JoinSessionResult);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Sessions Events")
	void OnStartSession(bool bWasSuccessful);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Multiplayer Sessions Events")
	void OnDestroySession(bool bWasSuccessful);
	
protected:
	virtual void BeginPlay() override;

private:
	UMultiplayerSessionsSubsystem* GetMultiplayerSessionsSubsystem() const;

	// Event binding functions
	void HandleCreateSessionComplete(bool bWasSuccessful);
	void HandleFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void HandleJoinSessionComplete(const FName& SessionName, EOnJoinSessionCompleteResult::Type Result);
	void HandleStartSessionComplete(bool bWasSuccessful);
	void HandleDestroySessionComplete(bool bWasSuccessful);
		
};
