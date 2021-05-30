#pragma once
#include "cameras/camera.hpp"

#define CAMERA_TYPE_RTS 0

class Camera3DPerspective : public Camera {
public:
    int cameraType;
    float pitch, yaw, targetPitch, targetYaw;
    float velocityLerpFactor;

    glm::vec3 targetPosition;
    glm::vec3 targetTarget;
    glm::vec3 forward;

    // (RTS)
    glm::vec2 startingMouseDragPosition;
    glm::vec3 cameraPosOnStartDrag;
    float targetTargetDistance;
    float targetDistance;
    float yawOnStartDrag, pitchOnStartDrag;
    float panningSpeed;
    float zoomingSpeed;

    Camera3DPerspective() : Camera() {};
    Camera3DPerspective(int cameraType, float fov, glm::vec3 initialPos, glm::vec3 intialTarget, glm::vec3 worldUp) : Camera(fov, initialPos, intialTarget, worldUp) {
        this->cameraType = cameraType;
        this->velocityLerpFactor = 10.0f;
        this->pitch = 0.0f;
        this->yaw = 0.0f;
        this->targetPitch = 0.0f;
        this->targetYaw = 0.0f;
        this->targetPosition = intialTarget;
        this->targetDistance = glm::distance(initialPos, intialTarget);
        this->targetTargetDistance = this->targetDistance;
        this->panningSpeed = 0.05f;
        this->zoomingSpeed = 4.0f;
    }

    glm::mat4 GetProjectionMatrix(float framebufferWidth, float framebufferHeight);
    glm::mat4 GetViewMatrix();
    void Update();
};