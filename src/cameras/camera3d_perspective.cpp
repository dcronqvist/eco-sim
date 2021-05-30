#include "cameras/camera3d_perspective.hpp"
#include "glad.h"
#include "glfw3.h"
#include "utils/input.hpp"
#include "utils/utility.hpp"


glm::mat4 Camera3DPerspective::GetProjectionMatrix(float framebufferWidth, float framebufferHeight) {
    return glm::perspective(glm::radians(this->fov), framebufferWidth / framebufferHeight, 0.1f, 500.0f);
}

glm::mat4 Camera3DPerspective::GetViewMatrix() {
    return glm::lookAt(this->cameraPos, this->cameraTarget, this->worldUp);
}

void Camera3DPerspective::Update() {
    this->cameraRight = glm::normalize(glm::cross(this->forward, this->worldUp));
    float camHeightFactor = (this->targetDistance / 100.0f);


    switch (this->cameraType) {
    case CAMERA_TYPE_RTS:
        this->forward = glm::vec3(sinf(yaw), 0.0f, cosf(yaw));

        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
            this->startingMouseDragPosition = Input::GetMousePosition();
            this->cameraPosOnStartDrag = this->targetPosition;
        }
        if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
            glm::vec2 mouseDragDiff = this->startingMouseDragPosition - Input::GetMousePosition();
            this->targetTarget = this->cameraPosOnStartDrag + (((this->forward * mouseDragDiff.y) + (this->cameraRight * -mouseDragDiff.x))) * panningSpeed * camHeightFactor;
        }

        float mouseWheelMove = Input::GetMouseWheelMove();
        this->targetTargetDistance -= mouseWheelMove * zoomingSpeed * camHeightFactor;
        this->targetTargetDistance = Utils::Clamp(this->targetTargetDistance, 1.0f, 100.0f);

        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            this->startingMouseDragPosition = Input::GetMousePosition();
            this->yawOnStartDrag = this->yaw;
            this->pitchOnStartDrag = this->pitch;
        }
        if (Input::IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) {
            glm::vec2 mouseDragDiff = this->startingMouseDragPosition - Input::GetMousePosition();
            this->targetYaw = this->yawOnStartDrag + mouseDragDiff.x / 100.0f;
            this->targetPitch = this->pitchOnStartDrag + mouseDragDiff.y / 100.0f;
        }
        this->targetPitch = Utils::Clamp(this->targetPitch, -3.1415f / 2.2f, -0.05f);

        this->targetDistance += (this->targetTargetDistance - this->targetDistance) * this->velocityLerpFactor * Utils::deltaTime;
        this->yaw += (this->targetYaw - this->yaw) * this->velocityLerpFactor * Utils::deltaTime;
        this->pitch += (this->targetPitch - this->pitch) * this->velocityLerpFactor * Utils::deltaTime;
        this->targetPosition += (this->targetTarget - this->targetPosition) * this->velocityLerpFactor * Utils::deltaTime;
        this->cameraPos.x = sinf(yaw) * this->targetDistance * cosf(pitch) + this->targetPosition.x;
        this->cameraPos.y = ((pitch <= 0.0f) ? 1 : -1) * sinf(pitch) * targetDistance * sinf(pitch) + this->targetPosition.y;
        this->cameraPos.z = cosf(yaw) * targetDistance * cosf(pitch) + this->targetPosition.z;
        this->cameraTarget = targetPosition;
        break;
    }
}