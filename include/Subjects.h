#pragma once

#include "../include/Observers.h"

#include <vector>

class Subject
{
private:
    std::vector<Observer*> observers_;
public:
    void AddObserver(Observer* observer);
    void RemoveObserver(Observer* observer);
};