#include "../include/StateMachine.h"

#include "../include/Types.h"
#include "../include/AssetManager.h"
#include "../include/AnimationManager.h"

#include <stdexcept>

using namespace std;
using namespace entt;

static void handleEventIdle(Animation& animation, EntityPosition& entityPosition, UnitState& state, GatheringFlags& flags, Event event);
static void handleEventWalking(Animation& animation, EntityPosition& entityPosition, UnitState& state, GatheringFlags& flags, Event event);
static void changeGatheringState(Animation& animation, EntityPosition& entityPosition, UnitState& state, Timer& timer, GatheringFlags& flags, Event event);
static void emptyGatheringFlags(GatheringFlags& flags);
static void checkTimer(UnitState& state, Timer& timer, GatheringFlags& flags);

void UnitStateMachine(registry& registry, entity entity)
{
    UnitState& state = registry.get<UnitState>(entity);
    GatheringFlags& gatheringFlags = registry.get<GatheringFlags>(entity);
    Timer& timer = registry.get<Timer>(entity);
    EventQueue& fifo = registry.get<EventQueue>(entity);
    Animation& animation = registry.get<Animation>(entity);
    EntityPosition& entityPosition = registry.get<EntityPosition>(entity);

    Event event = Event::NO_EVENT;

    if(!fifo.empty())
    {
        event = fifo.front();
        fifo.pop();
    }

    switch (state.Value)
    {
    case UnitState::IDLE:
    {
        handleEventIdle(animation, entityPosition, state, gatheringFlags, event);
        break;
    }

    case UnitState::WALKING:
    {
        handleEventWalking(animation, entityPosition, state, gatheringFlags, event);
        break;
    }

    case UnitState::GATHERING:
    {
        checkTimer(state, timer, gatheringFlags);
        changeGatheringState(animation, entityPosition, state, timer, gatheringFlags, event);
        break;
    }
    
    default:
        throw runtime_error("Unrecognized state");
        break;
    }
}

static void ChangeAnimation(Animation& animation, EntityPosition& entityPosition, string spritesheetType, float speed){
    AnimationManager* manager = AnimationManager::GetInstance();
    manager->ChangeAnimation(animation, entityPosition, spritesheetType, speed);
};

static void changeGatheringState(Animation& animation, EntityPosition& entityPosition, UnitState& state, Timer& timer, GatheringFlags& flags, Event event)
{
    if(event == Event::CLICKED_NEW_POSITION)
    {
        ChangeAnimation(animation, entityPosition, "peasant_gathering", 125.0f);      
        state.Value = UnitState::WALKING;
        timer.Stop();
        emptyGatheringFlags(flags);
    }
}

static void handleEventIdle(Animation& animation, EntityPosition& entityPosition, UnitState& state, GatheringFlags& flags, Event event)
{
    switch (event)
    {
    case CLICKED_ON_RESOURCE:
        flags.GatheringActivated = true;
        flags.SetGatheringPath = true;
        break;

    case CLICKED_NEW_POSITION:
        state.Value = UnitState::WALKING;
        ChangeAnimation(animation, entityPosition, "peasant_walking", 125.0f); 
        break;
    
    default:
        break;
    }
}

static void handleEventWalking(Animation& animation, EntityPosition& entityPosition, UnitState& state, GatheringFlags& flags, Event event)
{
    switch (event)
    {
    case Event::PATH_EMPTY:
        if(!flags.GatheringActivated)
        {
            state.Value = UnitState::IDLE;
            ChangeAnimation(animation, entityPosition, "peasant_idle", 50.0f); 
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