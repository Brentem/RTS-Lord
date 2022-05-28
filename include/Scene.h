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

inline EntityID CreateEntityId(EntityIndex index, EntityVersion version);
inline EntityIndex GetEntityIndex(EntityID id);
inline EntityVersion GetEntityVersion(EntityID id);
inline bool IsEntityValid(EntityID id);

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

        if(componentPools.size() <= componentId)
        {
            componentPools.resize(componentId + 1, nullptr);
        }

        if(componentPools[componentId] == nullptr)
        {
            componentPools[componentId] = new ComponentPool(sizeof(T));
        }

        T* pComponent = new (componentPools[componentId]->get(id)) T();

        entities[id].mask.set(componentId);

        return pComponent;
    }

    template <typename T>
    T* Get(EntityID id)
    {
        if(entities[GetEntityIndex(id)].id != id)
            return nullptr;

        int componentId = GetId<T>();

        if(!entities[id].mask.test(componentId))
            return nullptr;
        
        T* pComponent = static_cast<T*>(componentPools[componentId]->get(id));
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
