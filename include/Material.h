#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

struct Material {
    glm::vec3 ambient;     // 环境光反射系数
    glm::vec3 diffuse;     // 漫反射系数
    glm::vec3 specular;    // 镜面反射系数
    float shininess;       // 镜面高光的锐度

    Material(
        const glm::vec3& ambient = glm::vec3(0.2f),
        const glm::vec3& diffuse = glm::vec3(0.8f),
        const glm::vec3& specular = glm::vec3(1.0f),
        float shininess = 32.0f
    ) : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess) {}

    // 预定义的一些材质
    static Material Emerald() {
        return Material(
            glm::vec3(0.0215f, 0.1745f, 0.0215f),
            glm::vec3(0.07568f, 0.61424f, 0.07568f),
            glm::vec3(0.633f, 0.727811f, 0.633f),
            76.8f
        );
    }

    static Material Jade() {
        return Material(
            glm::vec3(0.135f, 0.2225f, 0.1575f),
            glm::vec3(0.54f, 0.89f, 0.63f),
            glm::vec3(0.316228f, 0.316228f, 0.316228f),
            12.8f
        );
    }

    static Material Gold() {
        return Material(
            glm::vec3(0.24725f, 0.1995f, 0.0745f),
            glm::vec3(0.75164f, 0.60648f, 0.22648f),
            glm::vec3(0.628281f, 0.555802f, 0.366065f),
            51.2f
        );
    }

    static Material Pearl() {
        return Material(
            glm::vec3(0.25f, 0.20725f, 0.20725f),
            glm::vec3(1.0f, 0.829f, 0.829f),
            glm::vec3(0.296648f, 0.296648f, 0.296648f),
            11.264f
        );
    }
};

#endif 