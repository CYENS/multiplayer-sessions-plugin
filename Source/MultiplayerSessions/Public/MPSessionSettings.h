

#pragma once

#include "CoreMinimal.h"
#include "MPSessionSettings.generated.h"

USTRUCT(BlueprintType)
struct MULTIPLAYERSESSIONS_API FMPSessionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	int32 PublicConnections = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	int32 PrivateConnections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bUseLAN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bAllowInvites;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bIsDedicatedServer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bUsePresence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bAllowJoinInProgress = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bAllowJoinViaPresence = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bUsesPresence = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bAllowJoinViaPresenceFriendsOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bAntiCheatProtected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bUsesStats = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bShouldAdvertise = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bUseLobbiesIfAvailable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bUseLobbiesVoiceChatIfAvailable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Settings")
	bool bStartAfterCreate;
};
