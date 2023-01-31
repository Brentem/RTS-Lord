#include "../include/Observers.h"

using namespace entt;

void UnitStateMachineObserver::OnNotify(registry& registry, entity& entity, Event event)
{
    UnitState& state = registry.get<UnitState>(entity);

    switch (event)
    {
    case MovementChanged:
        /* code */
        break;
    
    default:
        break;
    }
}