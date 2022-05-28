#pragma once

static int s_componentCounter = 0;
template <class T>
int GetId()
{
    static int s_componentId = s_componentCounter++;
    return s_componentId;
}