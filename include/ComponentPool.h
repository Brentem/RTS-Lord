#pragma once

#include <stdlib.h>

const int MAX_ENTITIES = 400;

struct ComponentPool
{
    ComponentPool(size_t elementSize);
    ~ComponentPool();
    inline void* get(size_t index)
    {
        return pData + index * elementSize;
    }

    char* pData {nullptr};
    size_t elementSize {0};
};