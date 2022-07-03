#include "../include/Pathfinding.h"

using namespace std;

vector<Pair> GetPath(vector<vector<Tile>>& grid, Pair start, Pair end)
{
    Cell startCell = {start, 0, 0, 0};
    Cell endCell = {end, 0, 0, 0};

    vector<Cell> openList;
    vector<Cell> closedList;

    openList.push_back(startCell);
}
