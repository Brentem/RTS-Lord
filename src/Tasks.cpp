#include "../include/Tasks.h"

#include "../include/Types.h"
#include "../include/Timer.h"

#include <stdexcept>
#include <vector>
#include <set>

using namespace std;

static vector<pair<entt::entity, Timer>> timers;

static bool reachedEndpoint(Vector2 currentPosition, Vector2 endPoint);
static Timer *getEntityTimer(entt::entity id);
static void deleteEntityTimer(entt::entity id);

void GatheringTask(Scene &scene)
{
    auto view = scene.registry.view<TaskState, EntityPosition, TaskPositions>();
    for (auto entity: view)
    {
        TaskState& state = view.get<TaskState>(entity);
        EntityPosition& position = view.get<EntityPosition>(entity);
        TaskPositions& taskPositions = view.get<TaskPositions>(entity);

        switch (state.Value)
        {
        case TaskState::IDLE:
        {
            // When clicked on resource
            if (true)
            {
                state.Value = TaskState::TO_RESOURCE;
            }
            break;
        }

        case TaskState::TO_RESOURCE:
        {
            if (reachedEndpoint(position.currentPosition, taskPositions.resourcePosition))
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
                state.Value = TaskState::TO_BASE;
            }
            break;
        }

        case TaskState::TO_BASE:
        {
            deleteEntityTimer(entity);

            if (reachedEndpoint(position.currentPosition, taskPositions.basePosition))
            {
                // Add to gold counter
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

static bool reachedEndpoint(Vector2 currentPosition, Vector2 endPoint)
{
    return (currentPosition.x == endPoint.x) && (currentPosition.y == endPoint.y);
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