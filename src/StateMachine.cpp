#include "../include/StateMachine.h"

#include "../include/Types.h"

#include <stdexcept>

using namespace std;
using namespace entt;

static void changeIdleState(UnitState& state, Event event);
static void handleEventWalking(UnitState& state, GatheringFlags& flags, Event event);
static void changeGatheringState(UnitState& state, Timer& timer, GatheringFlags& flags, Event event);
static void emptyGatheringFlags(GatheringFlags& flags);

void UnitStateMachine(registry& registry, entity entity, Event event)
{
    UnitState& state = registry.get<UnitState>(entity);
    GatheringFlags& gatheringFlags = registry.get<GatheringFlags>(entity);
    Timer& timer = registry.get<Timer>(entity);

    switch (state.Value)
    {
    case UnitState::IDLE:
        /* code */
        // RunIdleAnimation
        changeIdleState(state, event);
        break;

    case UnitState::WALKING:
        // RunWalkingAnimation
        handleEventWalking(state, gatheringFlags, event);
        break;

    case UnitState::GATHERING:
        // RunGatheringAnimation

        if(!timer.Started())
        {
            timer.Start(5);
        }

        timer.Update();

        if(timer.Finished())
        {
            gatheringFlags.GatheringDone = true;
            state.Value = UnitState::WALKING;
        }

        changeGatheringState(state, timer, gatheringFlags, event);
        break;
    
    default:
        throw runtime_error("Unrecognized state");
        break;
    }
}

static void changeIdleState(UnitState& state, Event event)
{
    if(event == Event::CLICKED_NEW_POSITION)
    {
        state.Value = UnitState::WALKING;
    }
}

static void changeGatheringState(UnitState& state, Timer& timer, GatheringFlags& flags, Event event)
{
    if(event == Event::CLICKED_NEW_POSITION)
    {
        state.Value = UnitState::WALKING;
        timer.Stop();
        emptyGatheringFlags(flags);
    }
}

static void handleEventWalking(UnitState& state, GatheringFlags& flags, Event event)
{
    switch (event)
    {
    case Event::PATH_EMPTY:
        state.Value = UnitState::IDLE;
        break;

    case Event::CLICKED_NEW_POSITION:
        state.Value = UnitState::WALKING;
        emptyGatheringFlags(flags);
        break;

    case Event::REACHED_RESOURCE:
        state.Value = UnitState::GATHERING;
        break;

    case Event::REACHED_BASE:
        flags.GatheringDone = false;
        break;

    default:
        break;
}
}

static void emptyGatheringFlags(GatheringFlags& flags)
{
    flags.GatheringActivated = false;
    flags.GatheringDone = false;
    flags.SetGatheringPath = false;
}