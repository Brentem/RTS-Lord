#pragma once

#include "../vendor/entt/entt.hpp"

#include "../include/Observers.h"

#include <vector>

class Subject
{
private:
    std::vector<Observer*> observers_;
public:
    void AddObserver(Observer* observer);
    void RemoveObserver(Observer* observer);
    // Maybe this(notify function) needs to be a protected function like in the example.
    void notify(entt::registry& registry, entt::entity entity, Event event); 
};