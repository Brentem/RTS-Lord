#pragma once

#include "../vendor/entt/entt.hpp"

#include "Types.h"

class Observer
{
public:
    virtual ~Observer() {};
    virtual void OnNotify(entt::registry& registry, entt::entity entity, Event event) = 0;
};

class UnitStateMachineObserver : public Observer
{
public:
    virtual void OnNotify(entt::registry& registry, entt::entity entity, Event event) override;
private:
    // https://stackoverflow.com/a/43379818
    void checkEventQueue(EventQueue& fifo, Event event);
};