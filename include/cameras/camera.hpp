#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
    public:
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 worldUp;
    float fov;

    protected:
    glm::vec3 cameraDirection;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;

    public:
    Camera();
    Camera(float fov, glm::vec3 initialPos, glm::vec3 initialTarget, glm::vec3 worldUp);
    virtual glm::mat4 GetProjectionMatrix(float framebufferWidth, float framebufferHeight) = 0;
    virtual glm::mat4 GetViewMatrix() = 0;
    virtual void Update() = 0;
};