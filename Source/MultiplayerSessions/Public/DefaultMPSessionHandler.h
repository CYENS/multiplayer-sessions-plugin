// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPSessionHandlerInterface.h"
#include "DefaultMPSessionHandler.generated.h"

class UMultiplayerSessionsSubsystem;

UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERSESSIONS_API UDefaultSessionHandler : public UObject, public IMPSessionHandler
{
	GENERATED_BODY()

public:
	UDefaultSessionHandler();

	// Set the subsystem instance
	void Initialize(UMultiplayerSessionsSubsystem* InSubsystem);

	// ISessionHandler interface
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Sessions")
	virtual void CreateSession(const int32 NumPublicConnections, FString MatchType) override;
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Sessions")
	virtual void FindSessions(const int32 MaxSearchResults) override;
	virtual void JoinSession(const FOnlineSessionSearchResult& SearchResult) override;
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Sessions")
	virtual void DestroySession() override;
	UFUNCTION(BlueprintCallable, Category = "Multiplayer Sessions")
	virtual bool StartSession() override;

private:
	TWeakObjectPtr<UMultiplayerSessionsSubsystem> MultiplayerSubsystem;
};
