#include "../include/ComponentPool.h"

ComponentPool::ComponentPool(size_t elementSize)
{
    this->elementSize = elementSize;
    pData = new char[elementSize * MAX_ENTITIES];
}

ComponentPool::~ComponentPool()
{
    delete[] pData;
}

inline void* ComponentPool::get(size_t index)
{
    return pData + index * elementSize;
}