#pragma once

#include "../vendor/entt/entt.hpp"

#include "Types.h"

class Observer
{
public:
    virtual ~Observer() {};
    virtual void OnNotify(entt::registry& registry, entt::entity& entity, Event event) = 0;
};

class UnitStateMachineObserver : public Observer
{
    virtual void OnNotify(entt::registry& registry, entt::entity& entity, Event event) override;
};