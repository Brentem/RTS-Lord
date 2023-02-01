#include "../include/Observers.h"

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

    UnitState& state = registry.get<UnitState>(entity);

    switch (event)
    {
    case IDLE:
        state.Value = UnitState::IDLE;
        cout << "TEST: Entity - " << (id_type)entity << " is set to IDLE" << endl;
        break;

    case WALKING:
        state.Value = UnitState::WALKING;
        cout << "TEST: Entity - " << (id_type)entity << " is set to WALKING" << endl;
        break;
    
    default:
        break;
    }
}