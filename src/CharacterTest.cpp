#include "../include/CharacterTest.h"



Character::Character(Texture2D texture,  Vector2 position, float width, float height){
    _width = width;
    _height = height;
    _texture = texture;
    _position = position;
    _targetPosition = position;
}

Character::~Character(){
}

void Character::setPosition(Vector2 position){
    _position = position;
}

Vector2 Character::getPosition(void){
    return _position;
}

void Character::setTargetPosition(Vector2 targetPosition){
    _targetPosition = (Vector2) {targetPosition.x - _width/2, targetPosition.y - _height/2};
}

Vector2 Character::getTargetPosition(void){
    return _targetPosition;
}

void Character::setIsSelected(bool isSelected){
    _isSelected = isSelected;
}
    
bool Character::getIsSelected(void){
    return _isSelected;
}

Rectangle Character::getRect(){
    return {0.0f, 0.0f, _width, _height};
}

Rectangle Character::getCollisionRect(){
    return {_position.x, _position.y, _width, _height};
}

void Character::updatePosition(){
    if(_targetPosition.x > _position.x){
        _position.x += 1.0f;
        if(_targetPosition.x < _position.x) _position.x = _targetPosition.x;
    }

    if(_targetPosition.x < _position.x){
        _position.x -= 1.0f;
        if(_targetPosition.x > _position.x) _position.x = _targetPosition.x;
    }

    if(_targetPosition.y > _position.y){
        _position.y += 1.0f;
        if(_targetPosition.y < _position.y) _position.y = _targetPosition.y;
    }

    if(_targetPosition.y < _position.y){
        _position.y -= 1.0f;
        if(_targetPosition.y > _position.y) _position.y = _targetPosition.y;
    }
}

void Character::updateIsSelected(Rectangle selectionRectangle){
    _isSelected = CheckCollisionRecs(selectionRectangle, getCollisionRect());
}


Vector2 CharacterTest_KeyboardInput(Vector2 characterPosition)
{
	if (IsKeyDown(KEY_D)) characterPosition.x += 1.0f;
    if (IsKeyDown(KEY_A)) characterPosition.x -= 1.0f;
    if (IsKeyDown(KEY_W)) characterPosition.y -= 1.0f;
    if (IsKeyDown(KEY_S)) characterPosition.y += 1.0f;
    return characterPosition;
}
