#pragma once


#include "Types.h"

class Observer
{
public:
    virtual ~Observer() {};
    virtual void OnNotify(EntityRegistry_t entityRegistry, Event event) = 0;
};

class UnitStateMachineObserver : public Observer
{
    virtual void OnNotify(EntityRegistry_t entityRegistry, Event event) override;
};