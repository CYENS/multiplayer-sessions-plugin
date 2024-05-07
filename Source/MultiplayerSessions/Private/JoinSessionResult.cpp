#include "JoinSessionResult.h"

EJoinSessionResult ConvertJoinResult(EOnJoinSessionCompleteResult::Type InType)
{
    switch (InType)
    {
    case EOnJoinSessionCompleteResult::Success:
        return EJoinSessionResult::Success;
    case EOnJoinSessionCompleteResult::SessionIsFull:
        return EJoinSessionResult::SessionIsFull;
    case EOnJoinSessionCompleteResult::SessionDoesNotExist:
        return EJoinSessionResult::SessionDoesNotExist;
    case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
        return EJoinSessionResult::CouldNotRetrieveAddress;
    case EOnJoinSessionCompleteResult::AlreadyInSession:
        return EJoinSessionResult::AlreadyInSession;
    default:
        return EJoinSessionResult::UnknownError;
    }
}
