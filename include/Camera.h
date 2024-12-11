#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // 相机属性
    glm::vec3 Position;
    glm::vec3 Target;     // 目标点（相机看向的点）
    glm::vec3 Up;
    
    // 轨道参数
    float Radius;         // 轨道半径
    float Yaw;           // 水平旋转角度
    float Pitch;         // 垂直旋转角度
    
    // 相机选项
    float MovementSpeed;  // 移动速度
    float RotationSpeed; // 旋转速度
    float ZoomSpeed;     // 缩放速度
    
    // 构造函数
    Camera(float radius = 5.0f);
    
    // 获取观察矩阵
    glm::mat4 GetViewMatrix() const;
    
    // 处理键盘输入
    void RotateLeft(float deltaTime);    // A键
    void RotateRight(float deltaTime);   // D键
    void RotateUp(float deltaTime);      // W键
    void RotateDown(float deltaTime);    // S键
    void ZoomIn(float deltaTime);        // +键
    void ZoomOut(float deltaTime);       // -键

private:
    // 更新相机位置
    void updateCameraPosition();
};

#endif 