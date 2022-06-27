#pragma once

#include <raylib.h>
#include <vector>

typedef struct
{
    int monitorWidth;
    int monitorHeight;
} MonitorSettings;

typedef struct {
    bool isWalkable;
}Tile;

typedef struct 
{
    int columnCount;
    int rowCount;
    int cellSize;
    int mapWidth;
    int mapHeight;
    Vector2 position;
    Vector2 offSet;
    std::vector<std::vector<Tile>> tiles;
}MapInfo;

typedef struct
{
    int upperBoundary;
    int lowerBoundary;
    int leftBoundary;
    int rightBoundary;
} Boundaries;

typedef struct
{
    Vector2 startPosition;
    Vector2 currentPosition;
    Vector2 worldStartPosition;
    Vector2 worldCurrentPosition;
    bool isdragging;
    bool isSelecting;
    bool giveNewTarget;
}MouseInfo;

struct EntityPosition
{
    Vector2 currentPosition;
    Vector2 targetPosition;
};