
#include "AutoSessionGameMode.h"

#include "AutoSessionPlayerController.h"

AAutoSessionGameMode::AAutoSessionGameMode(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	PlayerControllerClass = AAutoSessionPlayerController::StaticClass();
}
