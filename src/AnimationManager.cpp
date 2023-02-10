#include "../include/AnimationManager.h"

#include "../include/Types.h"
#include "../include/AssetManager.h"

#include <filesystem>
#include <vector>
#include <stdexcept>

using namespace std;

AnimationManager* AnimationManager::animationManagerInstance_{nullptr};
mutex AnimationManager::mutex_;

AnimationManager::AnimationManager()
{

}

AnimationManager::~AnimationManager()
{

}

AnimationManager* AnimationManager::GetInstance()
{
    lock_guard<mutex> lock(mutex_);

    if(animationManagerInstance_ == nullptr)
    {
        animationManagerInstance_ = new AnimationManager();
    }

    return animationManagerInstance_;
}

void AnimationManager::DeleteInstance()
{
    lock_guard<mutex> lock(mutex_);

    if(animationManagerInstance_ != nullptr)
    {
        delete animationManagerInstance_;
        animationManagerInstance_ = nullptr;
    }
}

void AnimationManager::ChangeAnimation(Animation& animation, EntityPosition& entityPosition, string spritesheetType, float speed){

        string spritesheet = "spritesheet_" + spritesheetType + "_" + Direction::toString(entityPosition.direction.Value) + ".png";
        AssetManager* assetManager = AssetManager::GetInstance();

        Animation newAnimation;
        newAnimation.width = animation.width;
        newAnimation.speed = speed;
        newAnimation.texture = assetManager->GetTexture(spritesheet);
        if(animation.currentSpritesheet == spritesheet){
           newAnimation.framesCounter = animation.framesCounter;
        } else{
            newAnimation.currentSpritesheet = spritesheet;
            newAnimation.width = animation.width;
            newAnimation.frameCount = newAnimation.texture.width/animation.width;
        }

        animation = newAnimation;
};

void AnimationManager::ChangeDirection(Animation& animation, Direction direction, string spritesheetType){
   string spritesheet = "spritesheet_" + spritesheetType + "_" + Direction::toString(direction.Value) + ".png";
   AssetManager* assetManager = AssetManager::GetInstance();

   animation.texture = assetManager->GetTexture(spritesheet);
}