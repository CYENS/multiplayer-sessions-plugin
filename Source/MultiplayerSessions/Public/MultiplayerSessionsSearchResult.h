// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MultiplayerSessionsSearchResult.generated.h"

USTRUCT(BlueprintType)
struct MULTIPLAYERSESSIONS_API FMultiplayerSessionsSearchResult
{
	GENERATED_BODY()

public:
	FMultiplayerSessionsSearchResult();

	// Extracted properties
	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString SessionId;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	FString OwnerName;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	int32 NumOpenPublicConnections;

	UPROPERTY(BlueprintReadOnly, Category = "Session")
	int32 MaxPublicConnections;

	// Set from FOnlineSessionSearchResult
	void SetFromOnlineResult(const FOnlineSessionSearchResult& OnlineResult);
};
