#include "../include/Tasks.h"

#include "../include/Timer.h"
#include "../include/2DMap.h"

#include <stdexcept>
#include <vector>
#include <set>

using namespace std;

static bool gridEqual(Pair pair1, Pair pair2);

void CheckResources(Scene& scene, MapInfo mapInfo, MouseInfo& mouseInfo)
{
    std::vector<Vector2> resourcePositions;

    auto resourceView = scene.registry.view<EntityPosition, EntityType>();
    for(auto entity: resourceView)
    {
        EntityPosition& position = resourceView.get<EntityPosition>(entity);
        EntityType& type = resourceView.get<EntityType>(entity);

        if(type.Value == EntityType::Resource)
        {
            resourcePositions.push_back(position.currentPosition);
        }
    }

    auto view = scene.registry.view<TaskPositions, SelectedCell, IsSelected>();
    for(auto entity: view)
    {
        TaskPositions& taskPosition = view.get<TaskPositions>(entity);
        SelectedCell& selectedCell = view.get<SelectedCell>(entity);
        IsSelected& isSelected = view.get<IsSelected>(entity);

        Pair selectedGrid = selectedCell.pair;

        if(isSelected.Value && mouseInfo.giveNewTarget)
        {
            for(auto resource: resourcePositions)
            {
                Pair resourceGrid = GetGridPosition(GetPositionOnMap(resource, mapInfo.mapWidth, mapInfo.mapHeight), 32);

                if(gridEqual(resourceGrid, selectedGrid))
                {
                    taskPosition.resourcePosition = resource;
                }
            }
        }
    }
}

void CheckBases(Scene& scene, MapInfo& mapInfo, MouseInfo& mouseInfo, vector<vector<Tile>>& grid)
{
    std::vector<Vector2> basePositions;

    auto baseView = scene.registry.view<EntityPosition, EntityType>();
    for(auto entity : baseView)
    {
        EntityPosition& position = baseView.get<EntityPosition>(entity);
        EntityType& type = baseView.get<EntityType>(entity);

        if(type.Value == EntityType::Building)
        {
            basePositions.push_back(position.currentPosition);
        }
    }

    auto unitView = scene.registry.view<TaskPositions, IsSelected>();
    for(auto entity : unitView)
    {
        IsSelected& isSelected = unitView.get<IsSelected>(entity);
        TaskPositions& taskPositions = unitView.get<TaskPositions>(entity);

        if(isSelected.Value && mouseInfo.giveNewTarget)
        {
            std::vector<Path> basePaths;

            Pair startGridCell = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.mapWidth, mapInfo.mapHeight), mapInfo.cellSize);
            
            for(auto basePos : basePositions)
            {
                Pair baseGrid = GetGridPosition(GetPositionOnMap(basePos, mapInfo.mapWidth, mapInfo.mapHeight), 32);
                Path path = GetPath(mapInfo, startGridCell, baseGrid, grid);
                basePaths.push_back(path);
            }

            Vector2 currentBasePosition = {0.0f, 0.0f};
            size_t pathSize = 0;

            for(size_t i = 0; i < basePaths.size(); i++)
            {
                if(i == 0)
                {
                    currentBasePosition = basePositions[i];
                    pathSize = basePaths[i].size();
                }

                if(basePaths[i].size() < pathSize)
                {
                    currentBasePosition = basePositions[i];
                    pathSize = basePaths[i].size();
                }
            }

            taskPositions.basePosition = currentBasePosition;
        }
    }
}

void CheckResourceClick(Scene& scene, Subject& subject, MapInfo mapInfo)
{
    auto view = scene.registry.view<TaskPositions, SelectedCell, GatheringFlags>();
    for(auto entity : view)
    {
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        SelectedCell& cell = view.get<SelectedCell>(entity);
        GatheringFlags& gatheringFlags = view.get<GatheringFlags>(entity);

        Pair resourceGrid = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair selectedGrid = cell.pair;

        if (gridEqual(resourceGrid, selectedGrid))
        {
            // gatheringFlags.GatheringActivated = true;
            // gatheringFlags.SetGatheringPath = true;
            subject.notify(scene.registry, entity, Event::CLICKED_ON_RESOURCE);
        }
    }
}

void CheckResourceReached(Scene& scene, Subject& subject, MapInfo mapInfo)
{
    auto view = scene.registry.view<TaskPositions, SelectedCell, EntityPosition>();

    for(auto entity : view)
    {
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        // SelectedCell& cell = view.get<SelectedCell>(entity);
        EntityPosition& position = view.get<EntityPosition>(entity);

        Pair resourceGrid = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair unitGrid = GetGridPosition(GetPositionOnMap(position.currentPosition, mapInfo.mapWidth, mapInfo.mapHeight), 32);

        if (gridEqual(unitGrid, resourceGrid))
        {
            subject.notify(scene.registry, entity, Event::REACHED_RESOURCE);
        }
    }
}

void CheckBaseReached(Scene& scene, Subject& subject, MapInfo mapInfo)
{
    auto view = scene.registry.view<TaskPositions, SelectedCell, EntityPosition>();

    for(auto entity : view)
    {
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        // SelectedCell& cell = view.get<SelectedCell>(entity);
        EntityPosition& position = view.get<EntityPosition>(entity);

        Pair baseGrid = GetGridPosition(GetPositionOnMap(taskPositions.basePosition, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair unitGrid = GetGridPosition(GetPositionOnMap(position.currentPosition, mapInfo.mapWidth, mapInfo.mapHeight), 32);

        if (gridEqual(unitGrid, baseGrid))
        {
            subject.notify(scene.registry, entity, Event::REACHED_BASE);
        }
    }
}

static bool gridEqual(Pair pair1, Pair pair2)
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}