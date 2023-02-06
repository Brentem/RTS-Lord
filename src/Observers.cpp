#include "../include/Observers.h"

#include "../include/StateMachine.h"

#include <iostream>

using namespace std;
using namespace entt;

void UnitStateMachineObserver::OnNotify(registry& registry, entity entity, Event event)
{
    EntityType& type = registry.get<EntityType>(entity);

    if(type.Value != EntityType::Worker)
    {
        return;
    }

    UnitStateMachine(registry, entity, event);
}