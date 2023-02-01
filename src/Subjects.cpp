#include "../include/Subjects.h"

using namespace entt;

void Subject::AddObserver(Observer* observer)
{
    if(observer == nullptr)
    {
        return;
    }

    observers_.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
    if(observer == nullptr)
    {
        return;
    }

    for(std::size_t i = 0; i < observers_.size(); i++)
    {
        if(observer == observers_[i])
        {
            observers_.erase(observers_.begin() + i);
        }
    }
}

void Subject::notify(registry& registry, entity entity, Event event)
{
    for(std::size_t i = 0; i < observers_.size(); i++)
    {
        observers_[i]->OnNotify(registry, entity, event);
    }
}