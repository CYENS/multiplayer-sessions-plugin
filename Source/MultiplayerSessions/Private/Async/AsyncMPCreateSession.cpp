#include "Async/AsyncMPCreateSession.h"

#include "MultiplayerSessionsSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

UAsyncMPCreateSession* UAsyncMPCreateSession::AsyncCreateSession(
	UObject* WorldContextObject,
	const FMPSessionSettings& SessionSettings
)
{
	UAsyncMPCreateSession* AsyncNode = NewObject<UAsyncMPCreateSession>();
	AsyncNode->WorldContextObject = WorldContextObject;
	AsyncNode->SessionSettings = SessionSettings;
	return AsyncNode;
}

void UAsyncMPCreateSession::Activate()
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

	
	HandleSuccessDelegateHandle = MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddUObject(this, &ThisClass::HandleSuccess);
	MultiplayerSessionsSubsystem->CreateSession(4, SessionSettings);
}


void UAsyncMPCreateSession::HandleSuccess(
	 FName SessionName,
	 FString SessionString,
	 bool bWasSuccessful
)
{
	OnSuccess.Broadcast(SessionString, TEXT(""));
	MultiplayerSessionsSubsystem->MultiplayerOnLoginComplete.Remove(HandleSuccessDelegateHandle);
	SetReadyToDestroy(); 
}

void UAsyncMPCreateSession::HandleFailure(const FString& ErrorMessage)
{
	// OnFailure.Broadcast(EmptySessionsArray, ErrorMessage);
	SetReadyToDestroy();
}
