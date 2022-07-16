#pragma once

#include <vector>
#include <bitset>

#include "ComponentPool.h"
#include "../include/ComponentId.h"

const int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;
typedef unsigned long long EntityID;


inline EntityID CreateEntityId(EntityIndex index, EntityVersion version)
{
  // Shift the index up 32, and put the version in the bottom
  return ((EntityID)index << 32) | ((EntityID)version);
}
inline EntityIndex GetEntityIndex(EntityID id)
{
  // Shift down 32 so we lose the version and get our index
  return id >> 32;
}
inline EntityVersion GetEntityVersion(EntityID id)
{
  // Cast to a 32 bit int to get our version number (loosing the top 32 bits)
  return (EntityVersion)id;
}
inline bool IsEntityValid(EntityID id)
{
  // Check if the index is our invalid index
  return (id >> 32) != EntityIndex(-1);
}

struct Scene
{
    struct EntityDesc
    {
        EntityID id;
        ComponentMask mask;
    };
    std::vector<EntityDesc> entities;
    std::vector<EntityIndex> freeEntities;
    std::vector<ComponentPool*> componentPools;

    EntityID NewEntity();
    void DestroyEntity(EntityID id);

    template <typename T>
    T* Assign(EntityID id)
    {
         if(entities[GetEntityIndex(id)].id != id)
            return nullptr;

        int componentId = GetId<T>();

        if(componentPools.size() <= (size_t)componentId) // Maybe make componentId a size_t variable
        {
            componentPools.resize(componentId + 1, nullptr);
        }

        if(componentPools[componentId] == nullptr)
        {
            componentPools[componentId] = new ComponentPool(sizeof(T));
        }


        EntityIndex index = GetEntityIndex(id);
        T* pComponent = new (componentPools[componentId]->get(index)) T();

        entities[id].mask.set(componentId);

        return pComponent;
    }

    template <typename T>
    T* Get(EntityID id)
    {
        EntityIndex index = GetEntityIndex(id);

        if(entities[index].id != id)
            return nullptr;

        int componentId = GetId<T>();

        if(!entities[id].mask.test(componentId))
            return nullptr;
        
        T* pComponent = static_cast<T*>(componentPools[componentId]->get(index));
        return pComponent;
    }

    template<typename T>
    void Remove(EntityID id)
    {
        if(entities[GetEntityIndex(id)].id != id)
            return;

        int componentId = GetId<T>();
        entities[GetEntityIndex(id)].mask.reset(componentId);
    }
};
