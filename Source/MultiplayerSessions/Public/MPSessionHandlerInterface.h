// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MPSessionHandlerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class MULTIPLAYERSESSIONS_API UMPSessionHandler: public UInterface
{
	GENERATED_BODY()
	
};

class MULTIPLAYERSESSIONS_API IMPSessionHandler
{
	GENERATED_BODY()

public:
	virtual void CreateSession(const int32 NumPublicConnections, const FString MatchType) = 0;
	virtual void FindSessions(const int32 MaxSearchResults) = 0;
	virtual void JoinSession(const FOnlineSessionSearchResult& SearchResult) = 0;
	virtual void DestroySession() = 0;
	virtual bool StartSession() = 0;
};
