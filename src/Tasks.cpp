#include "../include/Tasks.h"

#include "../include/Timer.h"
#include "../include/2DMap.h"

#include <stdexcept>
#include <vector>
#include <set>

using namespace std;

static vector<pair<entt::entity, Timer>> timers;

static bool gridEqual(Pair pair1, Pair pair2);
static Timer *getEntityTimer(entt::entity id);
static void deleteEntityTimer(entt::entity id);

void GatheringTask(Scene &scene, MouseInfo mouseInfo, MapInfo mapInfo)
{
    auto view = scene.registry.view<TaskState, EntityPosition, TaskPositions>();
    for (auto entity: view)
    {
        TaskState& state = view.get<TaskState>(entity);
        EntityPosition& position = view.get<EntityPosition>(entity);
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);

        Pair resourceGrid = GetGridPosition(GetPositionOnMap(taskPositions.resourcePosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair baseGrid = GetGridPosition(GetPositionOnMap(taskPositions.basePosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair unitGrid = GetGridPosition(GetPositionOnMap(position.currentPosition, mapInfo.offSet, mapInfo.mapWidth, mapInfo.mapHeight), 32);
        Pair selectedGrid = mouseInfo.gridCell;

        switch (state.Value)
        {
        case TaskState::IDLE:
        {
            // When clicked on resource
            if (gridEqual(resourceGrid, selectedGrid))
            {
                state.Value = TaskState::TO_RESOURCE;
            }
            break;
        }

        case TaskState::TO_RESOURCE:
        {
            if (gridEqual(unitGrid, resourceGrid))
            {
                Timer timer;
                timer.Start(5);

                pair<entt::entity, Timer> pair(entity, timer);
                timers.push_back(pair);

                state.Value = TaskState::GATHERING;
            }
            break;
        }

        case TaskState::GATHERING:
        {
            Timer *timer = getEntityTimer(entity);

            if (timer == nullptr)
            {
                runtime_error("If unit is in this state, timer should not be null!");
            }

            timer->Update();
            if (timer->Finished())
            {
                position.targetPosition = taskPositions.basePosition; // This should be changed to something better
                state.Value = TaskState::TO_BASE;
            }
            break;
        }

        case TaskState::TO_BASE:
        {
            deleteEntityTimer(entity);

            if (gridEqual(unitGrid, baseGrid))
            {
                // Add to gold counter
                position.targetPosition = taskPositions.resourcePosition; // This should be changed to something better
                state.Value = state.TO_RESOURCE;
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

static bool gridEqual(Pair pair1, Pair pair2)
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}

static Timer *getEntityTimer(entt::entity id)
{
    Timer *timer = nullptr;

    for (size_t i = 0; i < timers.size(); i++)
    {
        if (id == timers[i].first)
        {
            timer = &timers[i].second;
        }
    }

    return timer;
}

static void deleteEntityTimer(entt::entity id)
{
    for (size_t i = 0; i < timers.size(); i++)
    {
        if (id == timers[i].first)
        {
            timers.erase(timers.begin() + i);
        }
    }
}
