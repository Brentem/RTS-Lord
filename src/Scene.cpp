#include "../include/Scene.h"

#include "../include/ComponentId.h"

#define INVALID_ENTITY CreateEntityId(EntityIndex(-1), 0)

EntityID Scene::NewEntity()
{
    if(!freeEntities.empty())
    {
        EntityIndex newIndex = freeEntities.back();
        freeEntities.pop_back();
        EntityID newID = CreateEntityId(newIndex, GetEntityVersion(entities[newIndex].id));
        entities[newIndex].id = newID;
        return entities[newIndex].id;
    }

    entities.push_back({ CreateEntityId(EntityIndex(entities.size()), 0), ComponentMask() });
    return entities.back().id;
}

void Scene::DestroyEntity(EntityID id)
{
    EntityID newID = CreateEntityId(EntityIndex(-1), GetEntityVersion(id) + 1);
    entities[GetEntityIndex(id)].id = newID;
    entities[GetEntityIndex(id)].mask.reset();
    freeEntities.push_back(GetEntityIndex(id));
}