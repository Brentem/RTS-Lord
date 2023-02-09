#pragma once

extern "C"
{
    #include "raylib.h"
}

#include <queue>
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
    Vector2 currentPositionOnMap;
    Pair gridCell;
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

struct EntityType
{
    enum Value
    {
        Worker,
        Building,
        Resource
    } Value;
};

struct TaskPositions
{
    Vector2 basePosition;
    Vector2 resourcePosition;
};

struct UnitState
{
    enum Value
    {
        IDLE,
        WALKING,
        GATHERING
    } Value = IDLE;
};

struct Animation
{
    Texture2D texture;
    int frameCount = 0;
    int currentFrame = 0;
    float framesCounter = 0;
};

struct SelectedCell
{
    Pair pair;
};

struct IsSelected
{
    bool Value;

    IsSelected(bool value)
    {
        Value = value;
    }
};

enum Event
{
    NO_EVENT,
    PATH_EMPTY,
    CLICKED_NEW_POSITION,
    CLICKED_ON_RESOURCE,
    REACHED_RESOURCE,
    REACHED_BASE
};

typedef std::queue<Event> EventQueue;

struct GatheringFlags
{
    bool GatheringActivated;
    bool SetGatheringPath;
    bool SetBasePath;
    bool GatheringDone;

    GatheringFlags()
    {
        GatheringActivated = false;
        SetGatheringPath = false;
        SetBasePath = false;
        GatheringDone = false;
    }
};