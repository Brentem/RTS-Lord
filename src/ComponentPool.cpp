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
