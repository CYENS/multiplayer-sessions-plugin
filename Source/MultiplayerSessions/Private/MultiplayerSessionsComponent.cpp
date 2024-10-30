#include "MultiplayerSessionsComponent.h"

#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "JoinSessionResult.h"
#include "Algo/Transform.h"

UMultiplayerSessionsComponent::UMultiplayerSessionsComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UMultiplayerSessionsComponent::InitializeComponent()
{
    Super::InitializeComponent();

    if (UMultiplayerSessionsSubsystem* Subsystem = GetMultiplayerSessionsSubsystem())
    {
        Subsystem->MultiplayerOnCreateSessionComplete.AddUObject(this, &UMultiplayerSessionsComponent::HandleCreateSessionComplete);
        Subsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &UMultiplayerSessionsComponent::HandleFindSessionsComplete);
        Subsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &UMultiplayerSessionsComponent::HandleJoinSessionComplete);
        Subsystem->MultiplayerOnStartSessionComplete.AddUObject(this, &UMultiplayerSessionsComponent::HandleStartSessionComplete);
        Subsystem->MultiplayerOnDestroySessionComplete.AddUObject(this, &UMultiplayerSessionsComponent::HandleDestroySessionComplete);
    }
}

void UMultiplayerSessionsComponent::BeginPlay()
{
    Super::BeginPlay();
}

UMultiplayerSessionsSubsystem* UMultiplayerSessionsComponent::GetMultiplayerSessionsSubsystem() const
{
    if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
    {
        return GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
    }
    return nullptr;
}

void UMultiplayerSessionsComponent::HandleCreateSessionComplete(FName SessionName, FString SessionId, bool bWasSuccessful)
{
    OnCreateSessionComplete.Broadcast(bWasSuccessful);
    OnCreateSession(bWasSuccessful);
}

void UMultiplayerSessionsComponent::HandleFindSessionsComplete(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
    TArray<FMultiplayerSessionsSearchResult> BPSearchResults;
    Algo::Transform(
        SearchResults,
        BPSearchResults,
        [](const FOnlineSessionSearchResult& SearchResult)
        {
            FMultiplayerSessionsSearchResult BPSearchResult;
            BPSearchResult.SetFromOnlineResult(SearchResult);
            return BPSearchResult;
        }
    );
        
    OnFindSessionsComplete.Broadcast(BPSearchResults, bWasSuccessful);
    OnFindSessions(BPSearchResults, bWasSuccessful);
}

void UMultiplayerSessionsComponent::HandleJoinSessionComplete(const FName& SessionName, const EOnJoinSessionCompleteResult::Type Result)
{
    const EJoinSessionResult JoinSessionResult = ConvertJoinResult(Result);
    OnJoinSessionComplete.Broadcast(SessionName, JoinSessionResult);
    OnJoinSession(SessionName, JoinSessionResult);
}

void UMultiplayerSessionsComponent::HandleStartSessionComplete(bool bWasSuccessful)
{
    OnStartSessionComplete.Broadcast(bWasSuccessful);
    OnStartSession(bWasSuccessful);
}

void UMultiplayerSessionsComponent::HandleDestroySessionComplete(bool bWasSuccessful)
{
    OnDestroySessionComplete.Broadcast(bWasSuccessful);
    OnDestroySession(bWasSuccessful);
}
