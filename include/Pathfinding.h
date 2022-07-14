#pragma once

#include "Types.h"

#include <set>
#include <vector>

typedef std::pair<int, int> Pair;

struct Cell
{
    Cell* parent;
    Pair coordinates;
    double f, g, h;
};

std::vector<Pair> GetPath(Vector2 mapSize, Pair start, Pair end);