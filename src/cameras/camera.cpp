#include "cameras/camera.hpp"

Camera::Camera() {

}

Camera::Camera(float fov, glm::vec3 initialPos, glm::vec3 initialTarget, glm::vec3 worldUp) {
    this->fov = fov;
    this->cameraPos = initialPos;
    this->cameraTarget = initialTarget;
    this->worldUp = worldUp;
}