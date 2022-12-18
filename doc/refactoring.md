# Refactoring Ideas

## 2DMap.cpp
Split this file into multiple files, because it now handles background, mapinfo and user input.
User input should be its own file, the [Command](https://gameprogrammingpatterns.com/command.html) pattern can be used.

Interesting sources:

- [Designing a Robust Input Handling System for Games](https://www.gamedev.net/tutorials/_/technical/game-programming/designing-a-robust-input-handling-system-for-games-r2975/)
- [The 5 Golden Rules of Input](https://www.gamedeveloper.com/design/the-5-golden-rules-of-input)

## Scene.cpp
Maybe the name should be changed to Game and the MouseInfo, MapInfo etc. structs could be placed in this class.

[Singleton Design Pattern](https://gameprogrammingpatterns.com/singleton.html)

## Systems.cpp
"Private" functions can be placed in own file.
See if it is possible to make certain functions simpler.

## Tasks.cpp
Think of a better way to use the timers.
For adding gold to the counter the [Oberserver](https://gameprogrammingpatterns.com/observer.html) design pattern can be used.
Try to make the state machine simpler with use of the [State](https://gameprogrammingpatterns.com/state.html) design pattern.

## main.cpp
Try to get rid of function calls and creation of variables in the main function. Try to fit it to the [Game Loop](https://gameprogrammingpatterns.com/game-loop.html) design pattern. For instance game.HandleInput(), game.Update and game.Render.

## Other Sources
- [Creating a Map of Tiles // Game Engine series](https://youtu.be/mj7qCwHTtK8)
- [TIMESTEPS and DELTA TIME | Game Engine series](https://youtu.be/pctGOMDW-HQ)