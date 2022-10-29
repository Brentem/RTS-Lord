#include "../include/Tasks.h"

#include "../include/Timer.h"
#include "../include/2DMap.h"

#include <stdexcept>
#include <vector>
#include <set>

using namespace std;

// This is a bad practice, returning pointer to vector item
static vector<pair<entt::entity, Timer>> timers;

static bool gridEqual(Pair pair1, Pair pair2);
static Timer *getEntityTimer(entt::entity id);
static void deleteEntityTimer(entt::entity id);

void GatheringTask(Scene &scene, MouseInfo mouseInfo, MapInfo mapInfo)
{
    auto view = scene.registry.view<TaskState, EntityPosition, TaskPositions, SelectedCell, IsMoved, TaskStateChanged>();
    for(auto entity: view)
    {
        TaskState& state = view.get<TaskState>(entity);
        EntityPosition& position = view.get<EntityPosition>(entity);
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);
        SelectedCell& cell = view.get<SelectedCell>(entity);
        IsMoved& isMoved = view.get<IsMoved>(entity);
        TaskStateChanged& stateChanged = view.get<TaskStateChanged>(entity);

        Pair resourceGrid = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair baseGrid = GetGridPosition(GetPositionOnMap(taskPositions.basePosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair unitGrid = GetGridPosition(GetPositionOnMap(position.currentPosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair selectedGrid = cell.pair;

        switch (state.Value)
        {
        case TaskState::NOT_GATHERING:
        {
            stateChanged.Value = false;

            if (gridEqual(resourceGrid, selectedGrid))
            {
                state.Value = TaskState::TO_RESOURCE;
                stateChanged.Value = true;
            }

            isMoved.Value = false;
            break;
        }

        case TaskState::TO_RESOURCE:
        {
            stateChanged.Value = false;

            if (gridEqual(unitGrid, resourceGrid))
            {
                Timer timer;
                timer.Start(5);

                pair<entt::entity, Timer> pair(entity, timer);
                timers.push_back(pair);

                state.Value = TaskState::GATHERING;
                stateChanged.Value = true;
            }

            if(isMoved.Value)
            {
                isMoved = false;
                deleteEntityTimer(entity);
                state.Value = TaskState::NOT_GATHERING;
            }
            break;
        }

        case TaskState::GATHERING:
        {
            stateChanged.Value = false;
            Timer *timer = getEntityTimer(entity);

            if (timer == nullptr)
            {
                runtime_error("If unit is in this state, timer should not be null!");
            }

            timer->Update();
            if (timer->Finished())
            {
                state.Value = TaskState::TO_BASE;
                stateChanged.Value = true;
            }

            if(isMoved.Value)
            {
                isMoved = false;
                deleteEntityTimer(entity);
                state.Value = TaskState::NOT_GATHERING;
            }
            break;
        }

        case TaskState::TO_BASE:
        {
            stateChanged.Value = false;
            deleteEntityTimer(entity);

            if (gridEqual(unitGrid, baseGrid))
            {
                scene.gold += 10;
                state.Value = state.TO_RESOURCE;
                stateChanged.Value = true;
            }

            if(isMoved.Value)
            {
                isMoved = false;
                state.Value = TaskState::NOT_GATHERING;
            }
            break;
        }

        default:
        {
            throw runtime_error("Unrecognized state");
            break;
        }
        }
    }
}

void CheckResources(Scene& scene, MapInfo mapInfo)
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

        if(isSelected.Value)
        {
            for(auto resource: resourcePositions)
            {
                Pair resourceGrid = GetGridPosition(GetPositionOnMap(resource, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);

                if(gridEqual(resourceGrid, selectedGrid))
                {
                    taskPosition.resourcePosition = resource;
                }
            }
        }
    }
}

static bool gridEqual(Pair pair1, Pair pair2)
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}

// This is a bad practice, returning pointer to vector item
static Timer *getEntityTimer(entt::entity id)
{
    Timer *timer = nullptr;

    for(size_t i = 0; i < timers.size(); i++)
    {
        if(id == timers[i].first)
        {
            timer = &timers[i].second;
        }
    }

    return timer;
}

static void deleteEntityTimer(entt::entity id)
{
    for(size_t i = 0; i < timers.size(); i++)
    {
        if(id == timers[i].first)
        {
            timers.erase(timers.begin() + i);
        }
    }
}
