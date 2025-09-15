#include "GameTime.h"

GameTime::GameTime()
{

}

GameTime& GameTime::getInstance()
{
    static GameTime gt;
    return gt;
}

float GameTime::getDeltaTime()
{
    return GameTime::getInstance().deltaTime;
}

void GameTime::updateDeltaTime()
{
    float currentFrame = glfwGetTime();
    getInstance().deltaTime = currentFrame - getInstance().lastFrameTime;
    getInstance().lastFrameTime = currentFrame;
}