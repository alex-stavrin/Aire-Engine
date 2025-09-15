#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    public:
        Camera(const glm::vec3& startingPosition, float startingYaw, float startingPitch);

        glm::vec3 getPosition () {return position;};

        void cameraMouseInput(float xOffset, float yOffset);

        void moveCamera(const glm::vec3& direction);

        glm::vec3 getForward () {return glm::normalize(forward);};

        glm::vec3 getUp() {return glm::normalize(up);};

        glm::vec3 getRight();

    private:

        void calculateForward();

        // variables
        float yaw;
        float pitch;
        float speed = 2.0f;
        float sensitivity = 0.1f;
        glm::vec3 position;
        glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
        glm::vec3 forward = glm::vec3(0.0f,0.0f,-1.0f);
        bool invertPitch = true;
};