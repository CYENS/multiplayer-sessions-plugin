// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "BlueprintSessionResult.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct  MULTIPLAYERSESSIONS_API FBPSessionResult
{
	GENERATED_BODY()
	
	FOnlineSessionSearchResult SearchResult;
	
	UPROPERTY(BlueprintReadOnly)
	FString Id;
	UPROPERTY(BlueprintReadOnly)
	FString OwningUserName;
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, FString> SessionSettings;
	
};
