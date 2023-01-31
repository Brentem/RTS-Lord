#include "../include/StateMachine.h"

#include "../include/Types.h"

#include <stdexcept>

using namespace std;


void UnitStateMachine()
{
    UnitState state;

    switch (state.Value)
    {
    case UnitState::IDLE:
        /* code */
        // RunIdleAnimation
        break;

    case UnitState::WALKING:
        // RunWalkingAnimation
        break;

    case UnitState::GATHERING:
        // RunGatheringAnimation
        break;
    
    default:
        throw runtime_error("Unrecognized state");
        break;
    }
}