// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MultiplayerSessionsStyle.h"

class FMultiplayerSessionsCommands : public TCommands<FMultiplayerSessionsCommands>
{
public:

	FMultiplayerSessionsCommands()
		: TCommands<FMultiplayerSessionsCommands>(TEXT("MultiplayerSessions"), NSLOCTEXT("Contexts", "MultiplayerSessions", "MultiplayerSessions Plugin"), NAME_None, FMultiplayerSessionsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
