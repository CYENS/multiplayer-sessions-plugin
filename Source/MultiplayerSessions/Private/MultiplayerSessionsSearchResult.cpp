// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSearchResult.h"

#include "OnlineSessionSettings.h"

FMultiplayerSessionsSearchResult::FMultiplayerSessionsSearchResult()
:	NumOpenPublicConnections(0),
	MaxPublicConnections(0)
{
}

void FMultiplayerSessionsSearchResult::SetFromOnlineResult(const FOnlineSessionSearchResult& OnlineResult)
{
	SessionId = OnlineResult.Session.GetSessionIdStr();
	OwnerName = OnlineResult.Session.OwningUserName;
	NumOpenPublicConnections = OnlineResult.Session.NumOpenPublicConnections;
	MaxPublicConnections = OnlineResult.Session.SessionSettings.NumPublicConnections;
}
