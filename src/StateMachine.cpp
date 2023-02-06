#include "../include/StateMachine.h"

#include "../include/Types.h"

#include <stdexcept>

using namespace std;
using namespace entt;

static void handleEventIdle(UnitState& state, GatheringFlags& flags, Event event);
static void handleEventWalking(UnitState& state, GatheringFlags& flags, Event event);
static void changeGatheringState(UnitState& state, Timer& timer, GatheringFlags& flags, Event event);
static void emptyGatheringFlags(GatheringFlags& flags);
static void checkTimer(UnitState& state, Timer& timer, GatheringFlags& flags);

void UnitStateMachine(registry& registry, entity entity/*, Event event*/)
{
    UnitState& state = registry.get<UnitState>(entity);
    GatheringFlags& gatheringFlags = registry.get<GatheringFlags>(entity);
    Timer& timer = registry.get<Timer>(entity);
    EventQueue& fifo = registry.get<EventQueue>(entity);

    Event event = Event::NO_EVENT;

    if(!fifo.empty())
    {
        event = fifo.front();
        fifo.pop();
    }

    switch (state.Value)
    {
    case UnitState::IDLE:
        /* code */
        // RunIdleAnimation
        handleEventIdle(state, gatheringFlags, event);
        break;

    case UnitState::WALKING:
        // RunWalkingAnimation
        handleEventWalking(state, gatheringFlags, event);
        break;

    case UnitState::GATHERING:
        // RunGatheringAnimation
        checkTimer(state, timer, gatheringFlags);
        changeGatheringState(state, timer, gatheringFlags, event);
        break;
    
    default:
        throw runtime_error("Unrecognized state");
        break;
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

static void handleEventIdle(UnitState& state, GatheringFlags& flags, Event event)
{
    switch (event)
    {
    case CLICKED_ON_RESOURCE:
        flags.GatheringActivated = true;
        flags.SetGatheringPath = true;
        break;

    case CLICKED_NEW_POSITION:
        state.Value = UnitState::WALKING;
        break;
    
    default:
        break;
    }
}

static void handleEventWalking(UnitState& state, GatheringFlags& flags, Event event)
{
    switch (event)
    {
    case Event::PATH_EMPTY:
        if(!flags.GatheringActivated)
        {
            state.Value = UnitState::IDLE;
        }
        break;

    case CLICKED_ON_RESOURCE:
        if(!flags.GatheringActivated)
        {
            flags.GatheringActivated = true;
            flags.SetGatheringPath = true;
            flags.GatheringDone = false;
            flags.SetBasePath = false;
        }
        break;

    case Event::CLICKED_NEW_POSITION:
        state.Value = UnitState::WALKING;
        emptyGatheringFlags(flags);
        break;

    case Event::REACHED_RESOURCE:
        if((flags.GatheringActivated) && !(flags.GatheringDone))
        {
            state.Value = UnitState::GATHERING;
        }
        break;

    case Event::REACHED_BASE:
        if(flags.GatheringActivated)
        {
            flags.SetGatheringPath = true;
            flags.GatheringDone = false;
        }
        break;

    default:
        break;
}
}

static void emptyGatheringFlags(GatheringFlags& flags)
{
    flags.GatheringActivated = false;
    flags.SetGatheringPath = false;
    flags.SetBasePath = false;
    flags.GatheringDone = false;
}

static void checkTimer(UnitState& state, Timer& timer, GatheringFlags& flags)
{
    if(!flags.GatheringDone)
    {
        if(!timer.Started())
        {
            timer.Start(5);
        }

        timer.Update();

        if(timer.Finished())
        {
            flags.SetBasePath = true;
            flags.GatheringDone = true;
            state.Value = UnitState::WALKING;
        }
    }
}