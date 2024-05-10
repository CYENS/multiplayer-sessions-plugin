// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultMPSessionHandler.h"

#include "MultiplayerSessionsSubsystem.h"

UDefaultSessionHandler::UDefaultSessionHandler()
	: MultiplayerSubsystem(nullptr)
{
}

void UDefaultSessionHandler::Initialize(UMultiplayerSessionsSubsystem* InSubsystem)
{
	MultiplayerSubsystem = InSubsystem;
}

void UDefaultSessionHandler::CreateSession(const int32 NumPublicConnections, const FString MatchType)
{
	if (MultiplayerSubsystem.IsValid())
	{
		MultiplayerSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UDefaultSessionHandler::FindSessions(const int32 MaxSearchResults)
{
	if (MultiplayerSubsystem.IsValid())
	{
		MultiplayerSubsystem->FindSessions(MaxSearchResults);
	}
}

void UDefaultSessionHandler::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (MultiplayerSubsystem.IsValid())
	{
		MultiplayerSubsystem->JoinSession(SearchResult);
	}
}

void UDefaultSessionHandler::DestroySession()
{
	if (MultiplayerSubsystem.IsValid())
	{
		MultiplayerSubsystem->DestroySession();
	}
}

bool UDefaultSessionHandler::StartSession()
{
	return MultiplayerSubsystem.IsValid() ? MultiplayerSubsystem->StartSession() : false;
}
