#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AutoSessionGameMode.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API AAutoSessionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	AAutoSessionGameMode(const FObjectInitializer& ObjectInitializer);
	
public:
	UFUNCTION(BlueprintCallable, Category="AutoSession")
	void TravelToMap(const FName MapName)
	{
		if (HasAuthority())
		{
			if (UWorld* World = GetWorld())
			{
				World->ServerTravel(MapName.ToString() + TEXT("?listen"), true);
			}
		}
	}
};
