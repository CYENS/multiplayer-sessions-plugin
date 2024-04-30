// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerSessionsStyle.h"
#include "MultiplayerSessions.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMultiplayerSessionsStyle::StyleInstance = nullptr;

void FMultiplayerSessionsStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMultiplayerSessionsStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMultiplayerSessionsStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MultiplayerSessionsStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FMultiplayerSessionsStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MultiplayerSessionsStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MultiplayerSessions")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MultiplayerSessions.PluginAction", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	return Style;
}

void FMultiplayerSessionsStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMultiplayerSessionsStyle::Get()
{
	return *StyleInstance;
}
