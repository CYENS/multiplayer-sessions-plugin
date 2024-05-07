
#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "JoinSessionResult.generated.h"

UENUM(BlueprintType)
enum class EJoinSessionResult: uint8
{
	Success,
	SessionIsFull,
	SessionDoesNotExist,
	CouldNotRetrieveAddress,
	AlreadyInSession,
	UnknownError
};

EJoinSessionResult ConvertJoinResult(EOnJoinSessionCompleteResult::Type InType);
