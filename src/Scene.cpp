#include "../include/Scene.h"


#include "../include/Scene.h"
#include "../include/ComponentId.h"

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