// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerSessionsCommands.h"

#define LOCTEXT_NAMESPACE "FMultiplayerSessionsModule"

void FMultiplayerSessionsCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "MultiplayerSessions", "Execute MultiplayerSessions action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
