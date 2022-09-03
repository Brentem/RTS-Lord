#pragma once

class Timer
{
    private:
        float lifeTime;
        bool timerStarted;
    
    public:
        Timer();
        void Start(float lifeTime);
        void Update();
        bool Finished();
};