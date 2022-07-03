#pragma once

#include "Types.h"

#include <set>
#include <vector>

typedef std::pair<int, int> Pair;

struct Cell
{
    Pair parent;
    double f, g, h;
};

std::vector<Pair> GetPath(std::vector<std::vector<Tile>>& grid, Pair start, Pair end);