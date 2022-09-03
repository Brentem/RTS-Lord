#include "../include/Timer.h"

#include <raylib.h>

Timer::Timer()
{
    lifeTime = 0;
    timerStarted = false;
}

void Timer::Start(float lifeTime)
{
    this->lifeTime = lifeTime;
    timerStarted = true;
}

void Timer::Update()
{
    if(timerStarted)
    {
        lifeTime -= GetFrameTime();
    }
}

bool Timer::Finished()
{
    bool finished = false;

    if(timerStarted && (lifeTime <= 0))
    {
        timerStarted = false;
        finished = true;
    }

    return finished;
}