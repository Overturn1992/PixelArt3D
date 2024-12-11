#include "Model.h"
#include <GL/glew.h>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Model::Model() : VAO(0), VBO(0) {
}

Model::~Model() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
    }
}

void Model::setupMesh() {
    if (vertices.empty()) {
        std::cerr << "Warning: Trying to setup mesh with no vertices" << std::endl;
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // 顶点颜色
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    // 顶点法线
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glBindVertexArray(0);
}

void Model::draw() const {
    if (VAO == 0) {
        std::cerr << "Warning: Trying to draw model before setting up mesh" << std::endl;
        return;
    }
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Model::addCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color) {
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float w = size.x / 2.0f;
    float h = size.y / 2.0f;
    float d = size.z / 2.0f;

    // 顶点位置
    std::vector<glm::vec3> positions = {
        // 前面 (Z+)
        {x - w, y - h, z + d}, {x + w, y - h, z + d}, {x + w, y + h, z + d},
        {x - w, y - h, z + d}, {x + w, y + h, z + d}, {x - w, y + h, z + d},
        // 后面 (Z-)
        {x + w, y - h, z - d}, {x - w, y - h, z - d}, {x - w, y + h, z - d},
        {x + w, y - h, z - d}, {x - w, y + h, z - d}, {x + w, y + h, z - d},
        // 右面 (X+)
        {x + w, y - h, z + d}, {x + w, y - h, z - d}, {x + w, y + h, z - d},
        {x + w, y - h, z + d}, {x + w, y + h, z - d}, {x + w, y + h, z + d},
        // 左面 (X-)
        {x - w, y - h, z - d}, {x - w, y - h, z + d}, {x - w, y + h, z + d},
        {x - w, y - h, z - d}, {x - w, y + h, z + d}, {x - w, y + h, z - d},
        // 上面 (Y+)
        {x - w, y + h, z + d}, {x + w, y + h, z + d}, {x + w, y + h, z - d},
        {x - w, y + h, z + d}, {x + w, y + h, z - d}, {x - w, y + h, z - d},
        // 下面 (Y-)
        {x - w, y - h, z - d}, {x + w, y - h, z - d}, {x + w, y - h, z + d},
        {x - w, y - h, z - d}, {x + w, y - h, z + d}, {x - w, y - h, z + d}
    };

    // 每个面的法线
    std::vector<glm::vec3> normals = {
        // 前面
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        // 后面
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
        // 右面
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        // 左面
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
        // 上面
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        // 下面
        {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
        {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
    };

    // 添加所有顶点
    for (size_t i = 0; i < positions.size(); i++) {
        Vertex vertex;
        vertex.Position = positions[i];
        vertex.Color = color;
        vertex.Normal = normals[i];
        vertices.push_back(vertex);
    }
}

Model Model::createCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color) {
    Model model;
    model.addCube(position, size, color);
    model.setupMesh();
    return model;
}

Model Model::createGround(float width, float depth, const glm::vec3& color) {
    Model model;
    model.addCube(glm::vec3(0.0f, -0.1f, 0.0f), glm::vec3(width, 0.2f, depth), color);
    model.setupMesh();
    return model;
}

Model Model::createCat(const glm::vec3& position, float scale) {
    Model model;
    
    // 定义颜色
    glm::vec3 mainColor(0.6f, 0.6f, 0.6f);         // 主体灰色
    glm::vec3 darkColor(0.1f, 0.1f, 0.1f);         // 深色部分
    glm::vec3 accentColor(0.7f, 0.7f, 0.7f);       // 强调色
    glm::vec3 eyeWhite(1.0f, 1.0f, 1.0f);          // 眼睛白色部分
    glm::vec3 eyeBlack(0.0f, 0.0f, 0.0f);          // 眼睛黑色部分
    glm::vec3 innerEarColor(1.0f, 0.85f, 0.85f);   // 耳朵内部（粉嫩的肉色）
    glm::vec3 noseColor(1.0f, 0.6f, 0.7f);         // 鼻子颜色（可爱的粉���）
    glm::vec3 pawColor(0.85f, 0.75f, 0.7f);        // 爪子颜色（柔和的米色）

    // 身体主体
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, 0.0f),
        glm::vec3(scale * 0.45f, scale * 0.4f, scale * 0.9f),
        mainColor
    );
    
    // 身体两侧
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 0.5f, 0.0f),
        glm::vec3(scale * 0.35f, scale * 0.35f, scale * 0.85f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 0.5f, 0.0f),
        glm::vec3(scale * 0.35f, scale * 0.35f, scale * 0.85f),
        mainColor
    );
    
    // 上部填充
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.65f, 0.0f),
        glm::vec3(scale * 0.4f, scale * 0.2f, scale * 0.85f),
        mainColor
    );
    
    // 下部填充
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.3f, 0.0f),
        glm::vec3(scale * 0.42f, scale * 0.2f, scale * 0.85f),
        mainColor
    );
    
    // 前后填充
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, scale * 0.4f),
        glm::vec3(scale * 0.43f, scale * 0.38f, scale * 0.2f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, -scale * 0.4f),
        glm::vec3(scale * 0.43f, scale * 0.38f, scale * 0.2f),
        mainColor
    );
    
    // 角落填充
    for (float x : {-0.2f, 0.2f}) {
        for (float z : {-0.35f, 0.35f}) {
            model.addCube(
                position + glm::vec3(scale * x, scale * 0.5f, scale * z),
                glm::vec3(scale * 0.25f, scale * 0.35f, scale * 0.25f),
                mainColor
            );
        }
    }
    
    // 腹部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.4f, 0.0f),
        glm::vec3(scale * 0.4f, scale * 0.3f, scale * 0.8f),
        accentColor
    );
    
    // 腹部填充
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.35f, 0.0f),
        glm::vec3(scale * 0.38f, scale * 0.25f, scale * 0.75f),
        accentColor
    );
    
    // 腹部边缘填充
    for (float x : {-0.15f, 0.15f}) {
        model.addCube(
            position + glm::vec3(scale * x, scale * 0.38f, 0.0f),
            glm::vec3(scale * 0.2f, scale * 0.28f, scale * 0.7f),
            accentColor
        );
    }
    
    // 身体与腿部的连接
    for (float x : {-0.2f, 0.2f}) {
        for (float z : {-0.2f, 0.2f}) {
            model.addCube(
                position + glm::vec3(scale * x, scale * 0.3f, scale * z),
                glm::vec3(scale * 0.25f, scale * 0.2f, scale * 0.25f),
                mainColor
            );
        }
    }
    
    // 头部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.8f, scale * 0.5f),
        glm::vec3(scale * 0.4f, scale * 0.35f, scale * 0.4f),
        mainColor
    );
    
    // 面部前突
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.8f, scale * 0.7f),
        glm::vec3(scale * 0.35f, scale * 0.3f, scale * 0.25f),
        mainColor
    );
    
    // 面部轮廓
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.85f, scale * 0.8f),
        glm::vec3(scale * 0.3f, scale * 0.25f, scale * 0.15f),
        mainColor
    );
    
    // 鼻子
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.82f, scale * 0.88f),
        glm::vec3(scale * 0.12f, scale * 0.12f, scale * 0.08f),
        noseColor
    );
    
    // 眼睛部分
    // 左眼白色底色
    model.addCube(
        position + glm::vec3(-scale * 0.15f, scale * 0.9f, scale * 0.75f),
        glm::vec3(scale * 0.12f, scale * 0.15f, scale * 0.12f),
        eyeWhite
    );
    // 右眼白色底色
    model.addCube(
        position + glm::vec3(scale * 0.15f, scale * 0.9f, scale * 0.75f),
        glm::vec3(scale * 0.12f, scale * 0.15f, scale * 0.12f),
        eyeWhite
    );

    // 左眼黑色瞳孔
    model.addCube(
        position + glm::vec3(-scale * 0.15f, scale * 0.9f, scale * 0.77f),
        glm::vec3(scale * 0.09f, scale * 0.12f, scale * 0.08f),
        eyeBlack
    );
    // 右眼黑色瞳孔
    model.addCube(
        position + glm::vec3(scale * 0.15f, scale * 0.9f, scale * 0.77f),
        glm::vec3(scale * 0.09f, scale * 0.12f, scale * 0.08f),
        eyeBlack
    );

    // 眼睛高光
    // 主高光 - 左眼上方
    model.addCube(
        position + glm::vec3(-scale * 0.17f, scale * 0.93f, scale * 0.79f),
        glm::vec3(scale * 0.04f, scale * 0.04f, scale * 0.04f),
        eyeWhite
    );
    // 主高光 - 右眼上方
    model.addCube(
        position + glm::vec3(scale * 0.13f, scale * 0.93f, scale * 0.79f),
        glm::vec3(scale * 0.04f, scale * 0.04f, scale * 0.04f),
        eyeWhite
    );

    // 次高光 - 左眼中间
    model.addCube(
        position + glm::vec3(-scale * 0.13f, scale * 0.9f, scale * 0.79f),
        glm::vec3(scale * 0.03f, scale * 0.03f, scale * 0.04f),
        eyeWhite
    );
    // 次高光 - 右眼中间
    model.addCube(
        position + glm::vec3(scale * 0.17f, scale * 0.9f, scale * 0.79f),
        glm::vec3(scale * 0.03f, scale * 0.03f, scale * 0.04f),
        eyeWhite
    );

    // 小高光 - 左眼下方
    model.addCube(
        position + glm::vec3(-scale * 0.15f, scale * 0.87f, scale * 0.79f),
        glm::vec3(scale * 0.02f, scale * 0.02f, scale * 0.04f),
        eyeWhite
    );
    // 小高光 - 右眼下方
    model.addCube(
        position + glm::vec3(scale * 0.15f, scale * 0.87f, scale * 0.79f),
        glm::vec3(scale * 0.02f, scale * 0.02f, scale * 0.04f),
        eyeWhite
    );
    
    // 耳朵
    // 左耳
    model.addCube(
        position + glm::vec3(-scale * 0.18f, scale * 1.1f, scale * 0.5f),
        glm::vec3(scale * 0.15f, scale * 0.3f, scale * 0.15f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(-scale * 0.18f, scale * 1.1f, scale * 0.52f),
        glm::vec3(scale * 0.1f, scale * 0.25f, scale * 0.1f),
        innerEarColor
    );
    
    // 右耳
    model.addCube(
        position + glm::vec3(scale * 0.18f, scale * 1.1f, scale * 0.5f),
        glm::vec3(scale * 0.15f, scale * 0.3f, scale * 0.15f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.18f, scale * 1.1f, scale * 0.52f),
        glm::vec3(scale * 0.1f, scale * 0.25f, scale * 0.1f),
        innerEarColor
    );
    
    // 胡须基座
    // 左边基座
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 0.82f, scale * 0.85f),
        glm::vec3(scale * 0.08f, scale * 0.08f, scale * 0.08f),
        mainColor
    );
    // 右边基座
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 0.82f, scale * 0.85f),
        glm::vec3(scale * 0.08f, scale * 0.08f, scale * 0.08f),
        mainColor
    );

    // 左边胡须
    model.addCube(
        position + glm::vec3(-scale * 0.35f, scale * 0.85f, scale * 0.85f),
        glm::vec3(scale * 0.25f, scale * 0.02f, scale * 0.02f),
        eyeWhite
    );
    model.addCube(
        position + glm::vec3(-scale * 0.35f, scale * 0.82f, scale * 0.85f),
        glm::vec3(scale * 0.25f, scale * 0.02f, scale * 0.02f),
        eyeWhite
    );
    model.addCube(
        position + glm::vec3(-scale * 0.35f, scale * 0.79f, scale * 0.85f),
        glm::vec3(scale * 0.25f, scale * 0.02f, scale * 0.02f),
        eyeWhite
    );

    // 右边胡须
    model.addCube(
        position + glm::vec3(scale * 0.35f, scale * 0.85f, scale * 0.85f),
        glm::vec3(scale * 0.25f, scale * 0.02f, scale * 0.02f),
        eyeWhite
    );
    model.addCube(
        position + glm::vec3(scale * 0.35f, scale * 0.82f, scale * 0.85f),
        glm::vec3(scale * 0.25f, scale * 0.02f, scale * 0.02f),
        eyeWhite
    );
    model.addCube(
        position + glm::vec3(scale * 0.35f, scale * 0.79f, scale * 0.85f),
        glm::vec3(scale * 0.25f, scale * 0.02f, scale * 0.02f),
        eyeWhite
    );

    // 腿部
    // 前腿
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 0.2f, scale * 0.3f),
        glm::vec3(scale * 0.15f, scale * 0.3f, scale * 0.15f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 0.2f, scale * 0.3f),
        glm::vec3(scale * 0.15f, scale * 0.3f, scale * 0.15f),
        mainColor
    );
    
    // 后腿
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 0.2f, -scale * 0.3f),
        glm::vec3(scale * 0.15f, scale * 0.3f, scale * 0.15f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 0.2f, -scale * 0.3f),
        glm::vec3(scale * 0.15f, scale * 0.3f, scale * 0.15f),
        mainColor
    );
    
    // 爪子
    // 前爪
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 0.05f, scale * 0.3f),
        glm::vec3(scale * 0.12f, scale * 0.1f, scale * 0.12f),
        pawColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 0.05f, scale * 0.3f),
        glm::vec3(scale * 0.12f, scale * 0.1f, scale * 0.12f),
        pawColor
    );
    
    // 后爪
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 0.05f, -scale * 0.3f),
        glm::vec3(scale * 0.12f, scale * 0.1f, scale * 0.12f),
        pawColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 0.05f, -scale * 0.3f),
        glm::vec3(scale * 0.12f, scale * 0.1f, scale * 0.12f),
        pawColor
    );
    
    // 尾巴 - 使用多个立方体创建弧度
    // 尾巴根部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.6f, -scale * 0.6f),
        glm::vec3(scale * 0.12f, scale * 0.12f, scale * 0.25f),
        mainColor
    );

    // 尾巴中部（向上弯曲）
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.7f, -scale * 0.75f),
        glm::vec3(scale * 0.1f, scale * 0.1f, scale * 0.2f),
        mainColor
    );

    // 尾巴第三段（继续向上弯曲）
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.85f, -scale * 0.85f),
        glm::vec3(scale * 0.09f, scale * 0.09f, scale * 0.18f),
        mainColor
    );

    // 尾巴第四段（更细）
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.95f, -scale * 0.9f),
        glm::vec3(scale * 0.08f, scale * 0.08f, scale * 0.15f),
        mainColor
    );

    // 尾巴尖端（最细）
    model.addCube(
        position + glm::vec3(0.0f, scale * 1.0f, -scale * 0.95f),
        glm::vec3(scale * 0.06f, scale * 0.06f, scale * 0.1f),
        mainColor
    );

    // 尾巴连接处的平滑过渡
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.65f, -scale * 0.67f),
        glm::vec3(scale * 0.11f, scale * 0.11f, scale * 0.15f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.77f, -scale * 0.8f),
        glm::vec3(scale * 0.095f, scale * 0.095f, scale * 0.15f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.9f, -scale * 0.87f),
        glm::vec3(scale * 0.085f, scale * 0.085f, scale * 0.12f),
        mainColor
    );

    model.setupMesh();
    return model;
} 