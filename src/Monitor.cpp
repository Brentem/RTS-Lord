#include "../include/Monitor.h"

#include <raylib.h>

MonitorSettings Monitor_GetSettings(int viewportWidth, int viewportHeight)
{
    MonitorSettings settings = {0, 0};

    InitWindow(viewportWidth, viewportHeight, "RTS-Lord");

    int currentMonitor = GetCurrentMonitor();
    settings.monitorHeight = GetMonitorHeight(currentMonitor) - 100;
    settings.monitorWidth = GetMonitorWidth(currentMonitor) - 10;

    CloseWindow();

    return settings;
}