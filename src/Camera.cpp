#include "Camera.h"
#include <algorithm>

Camera::Camera(float radius) : 
    Target(glm::vec3(0.0f)),
    Up(glm::vec3(0.0f, 1.0f, 0.0f)),
    Radius(radius),
    Yaw(-45.0f),
    Pitch(30.0f),
    MovementSpeed(2.5f),
    RotationSpeed(50.0f),
    ZoomSpeed(2.0f)
{
    updateCameraPosition();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Target, Up);
}

void Camera::RotateLeft(float deltaTime) {
    Yaw += RotationSpeed * deltaTime;
    updateCameraPosition();
}

void Camera::RotateRight(float deltaTime) {
    Yaw -= RotationSpeed * deltaTime;
    updateCameraPosition();
}

void Camera::RotateUp(float deltaTime) {
    Pitch += RotationSpeed * deltaTime;
    Pitch = std::clamp(Pitch, -89.0f, 89.0f);
    updateCameraPosition();
}

void Camera::RotateDown(float deltaTime) {
    Pitch -= RotationSpeed * deltaTime;
    Pitch = std::clamp(Pitch, -89.0f, 89.0f);
    updateCameraPosition();
}

void Camera::ZoomIn(float deltaTime) {
    Radius -= ZoomSpeed * deltaTime;
    Radius = std::max(Radius, 1.0f); // 防止相机太近
    updateCameraPosition();
}

void Camera::ZoomOut(float deltaTime) {
    Radius += ZoomSpeed * deltaTime;
    Radius = std::min(Radius, 20.0f); // 防止相机太远
    updateCameraPosition();
}

void Camera::updateCameraPosition() {
    // 计算相机位置
    float x = Radius * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    float y = Radius * sin(glm::radians(Pitch));
    float z = Radius * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    
    Position = Target + glm::vec3(x, y, z);
} 