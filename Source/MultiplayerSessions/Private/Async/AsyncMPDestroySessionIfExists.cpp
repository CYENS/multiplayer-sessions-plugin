#include "Async/AsyncMPDestroySessionIfExists.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAsyncMPDestroySessionIfExists* UAsyncMPDestroySessionIfExists::AsyncDestroySessionIfExists(
	UObject* WorldContextObject
)
{
	UAsyncMPDestroySessionIfExists* AsyncNode = NewObject<UAsyncMPDestroySessionIfExists>();
	AsyncNode->WorldContextObject = WorldContextObject;
	return AsyncNode;
}

void UAsyncMPDestroySessionIfExists::Activate()
{
	if (!WorldContextObject)
	{
		HandleFailure(TEXT("Invalid World Context Object"));
		return;
	}

	MultiplayerSessionsSubsystem = WorldContextObject->GetWorld()->GetGameInstance()->GetSubsystem<UMultiplayerSessionsSubsystem>();
	if (!MultiplayerSessionsSubsystem)
	{
		HandleFailure(TEXT("Failed to get the MultiplayerSessionsSubsystem"));
		return;
	}
	
	HandleDestroyCompleteDelegateHandle = MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.AddUObject(this, &ThisClass::HandleDestroy);
	MultiplayerSessionsSubsystem->DestroySession();
}

void UAsyncMPDestroySessionIfExists::HandleDestroy(const bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		OnSuccess.Broadcast();
	}
	else
	{
		OnFailure.Broadcast();
	}
	MultiplayerSessionsSubsystem->MultiplayerOnDestroySessionComplete.Remove(HandleDestroyCompleteDelegateHandle);
	SetReadyToDestroy(); 
}

void UAsyncMPDestroySessionIfExists::HandleFailure(const FString& ErrorMessage)
{
	UE_LOG(LogMultiplayerSessionsSubsystem, Error, TEXT("%s"), *ErrorMessage);
	OnFailure.Broadcast();
	SetReadyToDestroy();
}
