#pragma once

#include "../include/Types.h"

#include <string>
#include <mutex>

using namespace std;

class AnimationManager{

private:
    static AnimationManager* animationManagerInstance_;
    static std::mutex mutex_;

protected:
    AnimationManager();
    ~AnimationManager();

public:
    AnimationManager(AnimationManager &other) = delete;
    void operator=(const AnimationManager &) = delete;
    static AnimationManager* GetInstance();
    static void DeleteInstance();
    void ChangeAnimation(Animation& animation, EntityPosition& entityPosition, std::string spritesheetType, float speed);
    void ChangeDirection(Animation& animation, Direction direction, string spritesheetType);
};