#include "../include/Pathfinding.h"

#include <cmath>

using namespace std;

static Pair childrenOffsets[8] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

static pair<Cell, int> findCurrentCellAndIndex(vector<Cell> openList);
static bool isGoalFound(vector<Pair>& path, Cell currentCell, Cell endCell);
static vector<Cell> generateChildren(Cell* currentCell);

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
        Cell currentCell = currentCellAndIndex.first;
        int index = currentCellAndIndex.second;

        openList.erase(openList.begin() + index);
        closedList.push_back(currentCell);

        vector<Pair> path;
        bool found = isGoalFound(path, currentCell, endCell);

        if(found)
        {
            // do something with path
            return path;
        }

        vector<Cell> children = generateChildren(&currentCell);

        for(Cell child: children)
        {
            int counter = 0;

            // Child is on the closed list
            for(Cell closedChild: closedList)
            {
                if((child.coordinates.first == closedChild.coordinates.first) &&
                (child.coordinates.second == closedChild.coordinates.second))
                {
                    counter++;
                }
            }

            if(counter > 0)
            {
                continue;
            }

            child.g = currentCell.g + 1;
            child.h = (pow((child.coordinates.first - endCell.coordinates.first), 2)) + 
                        (pow((child.coordinates.second - endCell.coordinates.second), 2));
            child.f = child.g + child.h;

            for(Cell openChild: openList)
            {
                if((child.coordinates.first == openChild.coordinates.first) &&
                (child.coordinates.second == openChild.coordinates.second) &&
                child.g > openChild.g)
                {
                    counter++;
                }
            }

            if(counter > 0)
            {
                continue;
            }

            openList.push_back(child);
        }
    }
}

static pair<Cell, int> findCurrentCellAndIndex(vector<Cell> openList)
{
    Cell currentCell = openList[0];
    int currentIndex = 0;

    for(size_t i = 0; i < openList.size(); i++)
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

static vector<Cell> generateChildren(Cell* currentCell)
{
    vector<Cell> children;
    // Handle nullptr case
    for(Pair childOffset: childrenOffsets)
    {
        Pair cellCoordinates = {(currentCell->coordinates.first + childOffset.first),
                                 (currentCell->coordinates.second + childOffset.second)};
        
        // Handle case if coordinates aren't available in grid

        // Handle case of not walkable terrain

        Cell newCell = {currentCell, cellCoordinates, 0, 0, 0};
        children.push_back(newCell);
    }

    return children;
}