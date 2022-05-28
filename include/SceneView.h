#pragma once

#include "Scene.h"

template<typename... ComponentTypes>
struct SceneView
{
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
      for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
        componentMask.set(componentIds[i]);
    }
  }

  struct Iterator
  {
      Iterator(Scene* pScene, EntityIndex index, ComponentMask mask, bool all) 
        : pScene(pScene), index(index), mask(mask), all(all) {}

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
        do
        {
          index++;
        } while (index < pScene->entities.size() && !ValidIndex());
        return *this;
      }

      EntityIndex index;
      Scene* pScene;
      ComponentMask mask;
      bool all{false};
  };
  
  const Iterator begin() const
  {
    int firstIndex = 0;
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

  Scene* pScene{ nullptr };
  ComponentMask componentMask;
  bool all{ false };
};