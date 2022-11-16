#pragma once

#include "Types.h"

#include <vector>

struct Cell
{
    Cell* parent;
    Pair coordinates;
    double f, g, h;
};

std::vector<Pair> GetPath(MapInfo mapInfo, Pair start, Pair end, std::vector<std::vector<Tile>>& grid);