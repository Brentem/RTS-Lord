#pragma once

extern "C"
{
    #include "raylib.h"
}

#include <vector>
#include <set>

#define MAX_UNITS_SELECTED 30

typedef std::pair<int, int> Pair;
typedef std::vector<Pair> Path;

struct MonitorSettings
{
    int monitorWidth;
    int monitorHeight;
};

struct Tile{
    bool isWalkable;
};

struct MapInfo
{
    int columnCount;
    int rowCount;
    int cellSize;
    int mapWidth;
    int mapHeight;
    Vector2 position;
    Vector2 offSet;
};

struct Boundaries
{
    int upperBoundary;
    int lowerBoundary;
    int leftBoundary;
    int rightBoundary;
};

struct MouseInfo
{
    Vector2 startPosition;
    Vector2 currentPosition;
    Vector2 worldStartPosition;
    Vector2 worldCurrentPosition;
    int selectedUnits;
    bool isdragging;
    bool isSelecting;
    bool giveNewTarget;
};

struct EntityPosition
{
    Vector2 currentPosition;
    Vector2 targetPosition;
};

struct EntitySize
{
    float width;
    float height;
};

struct TaskPositions
{
    Vector2 basePosition;
    Vector2 resourcePosition;
};

enum TaskState
{
    IDLE,
    TO_RESOURCE,
    GATHERING,
    TO_BASE
};