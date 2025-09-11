#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    public:
        Camera();

    private:
        float yaw;
        float pitch;
        float speed; 
        glm::vec3 position;
};