#include "Camera.h"
#include "GameTime.h"

Camera::Camera(const glm::vec3& startingPosition, float startingYaw, float startingPitch) : position(startingPosition),
yaw(startingYaw), pitch(startingPitch)
{

}

void Camera::cameraMouseInput(float xOffset, float yOffset)
{
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity * (invertPitch ? -1 : 1);

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    calculateForward();
}

void Camera::moveCamera(const glm::vec3& direction)
{
    float deltaTime = GameTime::getDeltaTime();
    position += direction * speed * deltaTime;
}

glm::vec3 Camera::getRight()
{
    return glm::normalize(glm::cross(forward, up));
}

void Camera::calculateForward()
{
    glm::vec3 newCameraForward;
    newCameraForward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newCameraForward.y = sin(glm::radians(pitch));
    newCameraForward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    forward = glm::normalize(newCameraForward);
}