#pragma once

#include "GLFW/glfw3.h"

class GameTime
{

private:
    GameTime();
    float deltaTime;
    float lastFrameTime;

public:

    static GameTime& getInstance();

    static float getDeltaTime();
    static void updateDeltaTime();
};