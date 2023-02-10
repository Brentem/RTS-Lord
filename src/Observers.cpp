#include "../include/Observers.h"

#include <iostream>

using namespace std;
using namespace entt;

void UnitStateMachineObserver::OnNotify(registry& registry, entity entity, Event event)
{
    EntityType& type = registry.get<EntityType>(entity);
    EventQueue& fifo = registry.get<EventQueue>(entity);

    if(type.Value != EntityType::Worker)
    {
        return;
    }

    checkEventQueue(fifo, event);
}

// https://stackoverflow.com/a/43379818
void UnitStateMachineObserver::checkEventQueue(EventQueue& fifo, Event event)
{
    EventQueue copyFifo = fifo;

    if(!copyFifo.empty())
    {
        auto item = copyFifo.front();
        copyFifo.pop();

        if(event == item)
        {
            return;
        }
    }

    fifo.push(event);
}