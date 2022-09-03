#include "../include/Tasks.h"

#include "../include/Types.h"
#include "../include/Timer.h"

#include <stdexcept>

void GatheringTask(Scene& scene)
{
    TaskState taskState = IDLE;
    Timer timer;

    switch(taskState)
    {
        case IDLE:
        //When clicked on resource
        taskState = TO_RESOURCE;
            break;

        case TO_RESOURCE:
        // When arrived at resource
        timer.Start(5);
        taskState = GATHERING;
            break;

        case GATHERING:
        timer.Update();

        if(timer.Finished())
        {
            taskState = TO_BASE;
        }
            break;

        case TO_BASE:
        // When arrived at base
        // Add to gold counter
        taskState = TO_RESOURCE;
            break;

        default:
            throw std::runtime_error("Unrecognized state");
            break;
    }
}