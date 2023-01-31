#include "../include/Subjects.h"

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

    for(size_t i = 0; i < observers_.size(); i++)
    {
        if(observer == observers_[i])
        {
            observers_.erase(observers_.begin() + i);
        }
    }
}