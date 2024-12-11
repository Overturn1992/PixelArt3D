#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

struct Light {
    glm::vec3 position;    // 光源位置
    glm::vec3 ambient;     // 环境光颜色
    glm::vec3 diffuse;     // 漫反射颜色
    glm::vec3 specular;    // 镜面反射颜色
    
    // 衰减参数
    float constant;        // 常数项
    float linear;          // 一次项
    float quadratic;       // 二次项

    Light(
        const glm::vec3& position = glm::vec3(0.0f),
        const glm::vec3& ambient = glm::vec3(0.2f),
        const glm::vec3& diffuse = glm::vec3(0.5f),
        const glm::vec3& specular = glm::vec3(1.0f),
        float constant = 1.0f,
        float linear = 0.09f,
        float quadratic = 0.032f
    ) : position(position), ambient(ambient), diffuse(diffuse), specular(specular),
        constant(constant), linear(linear), quadratic(quadratic) {}
};

#endif 