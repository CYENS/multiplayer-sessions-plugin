#pragma once

#include "CoreMinimal.h"
#include "MPSessionSettings.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncMPCreateSession.generated.h"

class UMultiplayerSessionsSubsystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSessionCreated, FString, SessionId, FString, ErrorMessage);

UCLASS()
class MULTIPLAYERSESSIONS_API UAsyncMPCreateSession : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MultiplayerSessions | Async ", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly =  "true"))
	static UAsyncMPCreateSession* AsyncCreateSession(
		UObject* WorldContextObject,
		const FMPSessionSettings& SessionSettings
	);
	
	UPROPERTY(BlueprintAssignable)
	FOnSessionCreated OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnSessionCreated OnFailure;


	virtual void Activate() override;

protected:

	UFUNCTION()
	void HandleSuccess(
	 FName SessionName,
	 FString SessionString,
	 bool bWasSuccessful
	);
	
	UFUNCTION()
	void HandleFailure(const FString& ErrorMessage);
	
private:
	FMPSessionSettings SessionSettings;
	
	UPROPERTY();
	UObject* WorldContextObject;
	
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	
	FDelegateHandle HandleSuccessDelegateHandle;
};
