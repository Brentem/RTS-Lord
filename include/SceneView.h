#pragma once

#include "Scene.h"

template<typename... ComponentTypes>
struct SceneView
{
  Scene* pScene{ nullptr };
  ComponentMask componentMask;
  bool all{ false };

  SceneView(Scene& scene) : pScene(&scene) 
  {
    if (sizeof...(ComponentTypes) == 0)
    {
      all = true;
    }
    else
    {
      // Unpack the template parameters into an initializer list
      int componentIds[] = { 0, GetId<ComponentTypes>() ... };
      for (size_t i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
        componentMask.set(componentIds[i]);
    }
  }

  struct Iterator
  {
      EntityIndex index;
      Scene* pScene;
      ComponentMask mask;
      bool all{false};

      Iterator(Scene* pScene, EntityIndex index, ComponentMask mask, bool all) 
        : index(index), pScene(pScene), mask(mask), all(all) {}

      EntityID operator*() const
      {
        return pScene->entities[index].id;
      }

      bool operator==(const Iterator& other) const
      {
        return index == other.index || index == pScene->entities.size();
      }

      bool operator!=(const Iterator& other) const
      {
        return index != other.index && index != pScene->entities.size();
      }

      bool ValidIndex()
      {
        return IsEntityValid(pScene->entities[index].id) &&
          (all || mask == (mask & pScene->entities[index].mask));
      }

      Iterator& operator++()
      {
        index++;
        return *this;
      }
  };
  
  const Iterator begin() const
  {
    size_t firstIndex = 0;
    while (firstIndex < pScene->entities.size() &&
    (componentMask != (componentMask & pScene->entities[firstIndex].mask) 
      || !IsEntityValid(pScene->entities[firstIndex].id))) 
    {
      firstIndex++;
    }
    return Iterator(pScene, firstIndex, componentMask, all);
  }

  const Iterator end() const
  {
    return Iterator(pScene, EntityIndex(pScene->entities.size()), componentMask, all);
  }
};