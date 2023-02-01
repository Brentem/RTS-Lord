#include "../include/Observers.h"

#include "../include/EntityRegistry.h"

using namespace entt;

void UnitStateMachineObserver::OnNotify(EntityRegistry_t entityRegistry, Event event)
{
    UnitState& state = entityRegistry.registry.get<UnitState>(entityRegistry.entity);

    switch (event)
    {
    case MovementChanged:
        /* code */
        break;
    
    default:
        break;
    }
}