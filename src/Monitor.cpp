#include "../include/Monitor.h"

extern "C"
{
    #include "raylib.h"
}

MonitorSettings Monitor_GetSettings(int viewportWidth, int viewportHeight)
{
    MonitorSettings settings = {0, 0};

    InitWindow(viewportWidth, viewportHeight, "RTS-Lord");

    int currentMonitor = GetCurrentMonitor();
    settings.monitorHeight = GetMonitorHeight(currentMonitor);
    settings.monitorWidth = GetMonitorWidth(currentMonitor);

    CloseWindow();

    return settings;
}