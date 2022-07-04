#include "../include/Pathfinding.h"

using namespace std;

static void findCurrentCell(vector<Cell> openList);
static bool isGoalFound(vector<Pair>& path, Cell currentCell, Cell endCell);

vector<Pair> GetPath(vector<vector<Tile>>& grid, Pair start, Pair end)
{
    Cell startCell = {nullptr, start, 0, 0, 0};
    Cell endCell = {nullptr, end, 0, 0, 0};

    vector<Cell> openList;
    vector<Cell> closedList;

    openList.push_back(startCell);

    while(openList.size() > 0)
    {
        pair<Cell, int> currentCellAndIndex = findCurrentCellAndIndex(openList);

        openList.erase(openList.begin() + currentCellAndIndex.second);
        closedList.push_back(currentCellAndIndex.first);

        vector<Pair> path;
        bool found = isGoalFound(path, currentCellAndIndex.first, endCell);

        if(found)
        {
            // do something with path
            return path;
        }
    }
}

static pair<Cell, int> findCurrentCellAndIndex(vector<Cell> openList)
{
    Cell currentCell = openList[0];
    int currentIndex = 0;

    for(int i = 0; i < openList.size(); i++)
    {
        if(openList[i].f < currentCell.f)
        {
            currentCell = openList[i];
            currentIndex = i;
        }
    }

    return pair<Cell, int>(currentCell, currentIndex);
}

static bool isGoalFound(vector<Pair>& path, Cell currentCell, Cell endCell)
{
    bool found = false;

    if((currentCell.coordinates.first == endCell.coordinates.first) &&
        (currentCell.coordinates.second == endCell.coordinates.second))
    {
        found = true;

        Cell* cellPointer = &currentCell;
        while(cellPointer != nullptr)
        {
            path.push_back(cellPointer->coordinates);
            cellPointer = cellPointer->parent;
        }
    }

    return found;
}