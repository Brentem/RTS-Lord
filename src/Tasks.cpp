// #include "../include/Tasks.h"

// #include "../include/Types.h"
// #include "../include/Timer.h"

// #include <stdexcept>
// #include <vector>
// #include <set>

// using namespace std;

// static vector<pair<EntityID, Timer>> timers;

// static bool reachedEndpoint(Vector2 currentPosition, Vector2 endPoint);
// static Timer *getEntityTimer(EntityID id);
// static void deleteEntityTimer(EntityID id);

// void GatheringTask(Scene &scene)
// {
//     for (EntityID ent : SceneView<TaskState, EntityPosition, TaskPositions>(scene))
//     {
//         TaskState *state = scene.Get<TaskState>(ent);
//         EntityPosition *position = scene.Get<EntityPosition>(ent);
//         TaskPositions *taskPositions = scene.Get<TaskPositions>(ent);

//         switch (*state)
//         {
//         case IDLE:
//             // When clicked on resource
//             if (true)
//             {
//                 *state = TO_RESOURCE;
//             }
//             break;

//         case TO_RESOURCE:
//             if (reachedEndpoint(position->currentPosition, taskPositions->resourcePosition))
//             {
//                 Timer timer;
//                 timer.Start(5);

//                 pair<EntityID, Timer> pair(ent, timer);

//                 *state = GATHERING;
//             }
//             break;

//         case GATHERING:
//             Timer *timer = getEntityTimer(ent);

//             if (timer == nullptr)
//             {
//                 runtime_error("If unit is in this state, timer should not be null!");
//             }

//             timer->Update();
//             if (timer->Finished())
//             {
//                 *state = TO_BASE;
//             }
//             break;

//         case TO_BASE:
//             deleteEntityTimer(ent);

//             if (reachedEndpoint(position->currentPosition, taskPositions->basePosition))
//             {
//                 // Add to gold counter
//                 *state = TO_RESOURCE;
//             }
//             break;

//         default:
//             throw runtime_error("Unrecognized state");
//             break;
//         }
//     }
// }

// static bool reachedEndpoint(Vector2 currentPosition, Vector2 endPoint)
// {
//     return (currentPosition.x == endPoint.x) && (currentPosition.y == endPoint.y);
// }

// static Timer *getEntityTimer(EntityID id)
// {
//     Timer *timer = nullptr;

//     for (size_t i = 0; i < timers.size(); i++)
//     {
//         if (id == timers[i].first)
//         {
//             timer = &timers[i].second;
//         }
//     }

//     return timer;
// }

// static void deleteEntityTimer(EntityID id)
// {
//     for (size_t i = 0; i < timers.size(); i++)
//     {
//         if (id == timers[i].first)
//         {
//             timers.erase(timers.begin() + i);
//         }
//     }
// }