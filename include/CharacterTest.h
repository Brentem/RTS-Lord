#ifndef CHARACTERTEST_H
#define CHARACTERTEST_H

#include "raylib.h"

#include "Types.h"

class Character
{
private:
    float _width, _height;
    Texture2D _texture;
    Vector2 _position;
    Vector2 _targetPosition = {0.0f, 0.0f};
    Rectangle _collisionRect;
    bool _isSelected = false;
public:
    Character(Texture2D texture,  Vector2 position, float width, float height);
    ~Character();

    void setPosition(Vector2 position);
    Vector2 getPosition();

    void setTargetPosition(Vector2 targetPosition);
    Vector2 getTargetPosition();

    void setIsSelected(bool isSelected);
    bool getIsSelected();

    Rectangle getRect();
    Rectangle getCollisionRect();

    void updatePosition();

    void updateIsSelected(Rectangle selectionRectangle);
};

Vector2 CharacterTest_KeyboardInput(Vector2 characterPosition);

// Vector2 CharacterTest_MoveToTarget(Vector2 characterPosition, Vector2 characterTargetPosition);

#endif