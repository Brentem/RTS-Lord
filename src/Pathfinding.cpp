#include "../include/Pathfinding.h"

#include <cmath>

using namespace std;

static Pair childrenOffsets[8] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

static pair<Cell*, int> findCurrentCellAndIndex(vector<Cell*> openList);
static bool isGoalFound(vector<Pair>& path, Cell* currentCell, Cell endCell);
static vector<Cell*> generateChildren(MapInfo mapInfo, Cell* currentCell, vector<vector<Tile>> grid);

static void deleteLists(vector<Cell*>& openList, vector<Cell*>& closedList);

vector<Pair> GetPath(MapInfo mapInfo, Pair start, Pair end, vector<vector<Tile>> grid)
{
    Cell* startCell = new Cell{nullptr, start, 0, 0, 0};
    Cell* endCell = new Cell{nullptr, end, 0, 0, 0};

    vector<Cell*> openList;
    vector<Cell*> closedList;

    vector<Pair> path;

    openList.push_back(startCell);

    while(openList.size() > 0)
    {
        pair<Cell*, int> currentCellAndIndex = findCurrentCellAndIndex(openList);
        Cell* currentCell = currentCellAndIndex.first;
        int index = currentCellAndIndex.second;

        openList.erase(openList.begin() + index);
        closedList.push_back(currentCell);

        bool found = isGoalFound(path, currentCell, *endCell);

        if(found)
        {
            deleteLists(openList, closedList);
            return path;
        }

        vector<Cell*> children = generateChildren(mapInfo, currentCell, grid);

        if(children.size() == 0)
        {
            deleteLists(openList, closedList);
            return path;
        }

        for(Cell* child: children)
        {
            int counter = 0;

            // Child is on the closed list
            for(Cell* closedChild: closedList)
            {
                if((child->coordinates.first == closedChild->coordinates.first) &&
                (child->coordinates.second == closedChild->coordinates.second))
                {
                    counter++;
                }
            }

            if(counter > 0)
            {
                delete child;
                continue;
            }

            child->g = currentCell->g + 1;
            child->h = (pow((child->coordinates.first - endCell->coordinates.first), 2)) + 
                        (pow((child->coordinates.second - endCell->coordinates.second), 2));
            child->f = child->g + child->h;

            for(Cell* openChild: openList)
            {
                if((child->coordinates.first == openChild->coordinates.first) &&
                (child->coordinates.second == openChild->coordinates.second) &&
                child->g > openChild->g)
                {
                    counter++;
                }
            }

            if(counter > 0)
            {
                delete child;
                continue;
            }

            openList.push_back(child);
        }
    }
    // Return empty list
    return path;
}

static pair<Cell*, int> findCurrentCellAndIndex(vector<Cell*> openList)
{
    Cell* currentCell = openList[0];
    int currentIndex = 0;

    for(size_t i = 0; i < openList.size(); i++)
    {
        if(openList[i]->f < currentCell->f)
        {
            currentCell = openList[i];
            currentIndex = i;
        }
    }

    return pair<Cell*, int>(currentCell, currentIndex);
}

static bool isGoalFound(vector<Pair>& path, Cell* currentCell, Cell endCell)
{
    bool found = false;

    if(currentCell == nullptr)
    {
        return found;
    }

    if((currentCell->coordinates.first == endCell.coordinates.first) &&
        (currentCell->coordinates.second == endCell.coordinates.second))
    {
        found = true;

        Cell* cellPointer = currentCell;
        while(cellPointer != nullptr)
        {
            path.push_back(cellPointer->coordinates);
            cellPointer = cellPointer->parent;
        }
    }

    return found;
}

static vector<Cell*> generateChildren(MapInfo mapInfo, Cell* currentCell, vector<vector<Tile>> grid)
{
    vector<Cell*> children;
    if(currentCell == nullptr)
    {
        return children;
    }

    for(Pair childOffset: childrenOffsets)
    {
        Pair cellCoordinates = {(currentCell->coordinates.first + childOffset.first),
                                 (currentCell->coordinates.second + childOffset.second)};
        
        // Handle case if coordinates aren't available in grid
        if((cellCoordinates.first >= mapInfo.columnCount) ||
            (cellCoordinates.second >= mapInfo.rowCount))
        {
            continue;
        }

        // Handle case of not walkable terrain
        // if(grid[cellCoordinates.first][cellCoordinates.second].isWalkable == false)
        // {
        //     continue;
        // }

        Cell* newCell = new Cell{currentCell, cellCoordinates, 0, 0, 0};
        children.push_back(newCell);
    }

    return children;
}

static void deleteLists(vector<Cell*>& openList, vector<Cell*>& closedList)
{
    for(Cell* item: openList)
    {
        delete item;
    }

    for(Cell* item: closedList)
    {
        delete item;
    }
}