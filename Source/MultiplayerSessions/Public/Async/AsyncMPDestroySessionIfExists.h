#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncMPDestroySessionIfExists.generated.h"

class UMultiplayerSessionsSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionDestroyed);

UCLASS()
class MULTIPLAYERSESSIONS_API UAsyncMPDestroySessionIfExists : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "MultiplayerSessions | Async ", meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly =  "true"))
	static UAsyncMPDestroySessionIfExists* AsyncDestroySessionIfExists(UObject* WorldContextObject);
	
	UPROPERTY(BlueprintAssignable)
	FOnSessionDestroyed OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnSessionDestroyed OnFailure;

	virtual void Activate() override;

protected:

	UFUNCTION()
	void HandleDestroy(const bool bWasSuccessful);
	
	UFUNCTION()
	void HandleFailure(const FString& ErrorMessage);
	
private:
	UPROPERTY();
	UObject* WorldContextObject;
	
	UPROPERTY()
	UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	
	FDelegateHandle HandleDestroyCompleteDelegateHandle;
};
