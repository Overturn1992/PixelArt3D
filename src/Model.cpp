#include "Model.h"
#include <GL/glew.h>
#include <iostream>

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
        // 前面 (6个顶点)
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        // 后面 (6个顶点)
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
        // 右面 (6个顶点)
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        // 左面 (6个顶点)
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
        // 上面 (6个顶点)
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        // 下面 (6个顶点)
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
    
    // 定义更明亮可爱的颜色
    glm::vec3 mainColor(0.6f, 0.6f, 0.6f);         // 主体灰色
    glm::vec3 darkColor(0.1f, 0.1f, 0.1f);         // 深色部分
    glm::vec3 accentColor(0.7f, 0.7f, 0.7f);       // 强调色
    glm::vec3 eyeWhite(1.0f, 1.0f, 1.0f);          // 眼睛白色部分
    glm::vec3 eyeBlack(0.0f, 0.0f, 0.0f);          // 眼睛黑色部分
    glm::vec3 innerEarColor(1.0f, 0.85f, 0.85f);   // 耳朵内部（粉嫩的肉色）
    glm::vec3 noseColor(1.0f, 0.6f, 0.7f);         // 鼻子颜色（可爱的粉红）
    glm::vec3 pawColor(0.85f, 0.75f, 0.7f);        // 爪子颜色（柔和的米色）
    
    // 身体主体 - 使用更匀称的比例
    // 主体核心 - 减小宽度，保持适当的高度和长度
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, 0.0f),
        glm::vec3(scale * 0.45f, scale * 0.4f, scale * 0.9f),
        mainColor
    );
    
    // 身体两侧 - 更薄的侧面
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
    
    // 上部填充 - 更窄的上部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.65f, 0.0f),
        glm::vec3(scale * 0.4f, scale * 0.2f, scale * 0.85f),
        mainColor
    );
    
    // 下部填充 - 适当的下部宽度
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.3f, 0.0f),
        glm::vec3(scale * 0.42f, scale * 0.2f, scale * 0.85f),
        mainColor
    );
    
    // 前后填充 - 更合适的厚度
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
    
    // 角落填充 - 更小的填充块
    for (float x : {-0.2f, 0.2f}) {
        for (float z : {-0.35f, 0.35f}) {
            model.addCube(
                position + glm::vec3(scale * x, scale * 0.5f, scale * z),
                glm::vec3(scale * 0.25f, scale * 0.35f, scale * 0.25f),
                mainColor
            );
        }
    }
    
    // 腹部 - 更窄的腹部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.4f, 0.0f),
        glm::vec3(scale * 0.4f, scale * 0.3f, scale * 0.8f),
        accentColor
    );
    
    // 腹部填充 - 适当的厚度
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.35f, 0.0f),
        glm::vec3(scale * 0.38f, scale * 0.25f, scale * 0.75f),
        accentColor
    );
    
    // 腹部边缘填充 - 更薄的边缘
    for (float x : {-0.15f, 0.15f}) {
        model.addCube(
            position + glm::vec3(scale * x, scale * 0.38f, 0.0f),
            glm::vec3(scale * 0.2f, scale * 0.28f, scale * 0.7f),
            accentColor
        );
    }
    
    // 身体与腿部的连接 - 更窄的连接
    for (float x : {-0.2f, 0.2f}) {
        for (float z : {-0.2f, 0.2f}) {
            model.addCube(
                position + glm::vec3(scale * x, scale * 0.3f, scale * z),
                glm::vec3(scale * 0.25f, scale * 0.2f, scale * 0.25f),
                mainColor
            );
        }
    }
    
    // 头部 - 更加突出的面部特征
    // 主要头部结构
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
    
    // 鼻子部分 - 更突出
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.82f, scale * 0.88f),
        glm::vec3(scale * 0.12f, scale * 0.12f, scale * 0.08f),
        noseColor
    );
    
    // 眼睛部分 - 使用高对比度的黑白配色
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
    model.addCube(
        position + glm::vec3(-scale * 0.17f, scale * 0.92f, scale * 0.79f),
        glm::vec3(scale * 0.04f, scale * 0.04f, scale * 0.04f),
        eyeWhite
    );
    model.addCube(
        position + glm::vec3(scale * 0.13f, scale * 0.92f, scale * 0.79f),
        glm::vec3(scale * 0.04f, scale * 0.04f, scale * 0.04f),
        eyeWhite
    );
    
    // 耳朵 - 更大更突出
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
    
    // 脸颊 - 增加立体感
    model.addCube(
        position + glm::vec3(-scale * 0.15f, scale * 0.8f, scale * 0.75f),
        glm::vec3(scale * 0.15f, scale * 0.2f, scale * 0.2f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.15f, scale * 0.8f, scale * 0.75f),
        glm::vec3(scale * 0.15f, scale * 0.2f, scale * 0.2f),
        mainColor
    );
    
    // 胡须基座
    model.addCube(
        position + glm::vec3(-scale * 0.15f, scale * 0.8f, scale * 0.85f),
        glm::vec3(scale * 0.08f, scale * 0.08f, scale * 0.08f),
        mainColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.15f, scale * 0.8f, scale * 0.85f),
        glm::vec3(scale * 0.08f, scale * 0.08f, scale * 0.08f),
        mainColor
    );
    
    // 耳朵内部 - 更粉嫩
    float innerEarScale = 0.8f;
    // 左耳内部
    model.addCube(
        position + glm::vec3(-scale * 0.18f, scale * 1.1f, scale * 0.52f),
        glm::vec3(scale * 0.1f, scale * 0.25f, scale * 0.1f),
        innerEarColor
    );
    // 右耳内部
    model.addCube(
        position + glm::vec3(scale * 0.18f, scale * 1.1f, scale * 0.52f),
        glm::vec3(scale * 0.1f, scale * 0.25f, scale * 0.1f),
        innerEarColor
    );
    
    // 头部 - 面部标记（深色部分）
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.9f, scale * 0.65f),
        glm::vec3(scale * 0.35f, scale * 0.3f, scale * 0.12f),
        darkColor
    );
    
    // 耳朵外部
    // 左耳
    model.addCube(
        position + glm::vec3(-scale * 0.2f, scale * 1.2f, scale * 0.45f),
        glm::vec3(scale * 0.15f, scale * 0.25f, scale * 0.12f),
        mainColor
    );
    // 右耳
    model.addCube(
        position + glm::vec3(scale * 0.2f, scale * 1.2f, scale * 0.45f),
        glm::vec3(scale * 0.15f, scale * 0.25f, scale * 0.12f),
        mainColor
    );
    
    // 腿部 - 使用更粗壮的实心立方体
    float legHeight = scale * 0.4f;
    float legWidth = scale * 0.18f;
    
    // 前腿 - 更粗壮
    // 左前腿
    model.addCube(
        position + glm::vec3(-scale * 0.25f, scale * 0.25f, scale * 0.25f),
        glm::vec3(scale * 0.18f, scale * 0.5f, scale * 0.18f),
        mainColor
    );
    // 右前腿
    model.addCube(
        position + glm::vec3(scale * 0.25f, scale * 0.25f, scale * 0.25f),
        glm::vec3(scale * 0.18f, scale * 0.5f, scale * 0.18f),
        mainColor
    );
    
    // 后腿 - 更粗壮
    float backLegWidth = scale * 0.22f;
    // 左后腿
    model.addCube(
        position + glm::vec3(-scale * 0.25f, scale * 0.25f, -scale * 0.25f),
        glm::vec3(backLegWidth, scale * 0.5f, backLegWidth),
        mainColor
    );
    // 右后腿
    model.addCube(
        position + glm::vec3(scale * 0.25f, scale * 0.25f, -scale * 0.25f),
        glm::vec3(backLegWidth, scale * 0.5f, backLegWidth),
        mainColor
    );
    
    // 腿部连接处填充
    // 前腿连接
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.45f, scale * 0.25f),
        glm::vec3(scale * 0.6f, scale * 0.2f, scale * 0.2f),
        mainColor
    );
    // 后腿连接
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.45f, -scale * 0.25f),
        glm::vec3(scale * 0.6f, scale * 0.2f, scale * 0.25f),
        mainColor
    );
    
    // 爪子 - 更大更实心
    float pawSize = scale * 0.15f;
    // 前爪
    model.addCube(
        position + glm::vec3(-scale * 0.25f, scale * 0.08f, scale * 0.25f),
        glm::vec3(scale * 0.2f, pawSize, scale * 0.2f),
        pawColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.25f, scale * 0.08f, scale * 0.25f),
        glm::vec3(scale * 0.2f, pawSize, scale * 0.2f),
        pawColor
    );
    
    // 后爪
    model.addCube(
        position + glm::vec3(-scale * 0.25f, scale * 0.08f, -scale * 0.25f),
        glm::vec3(scale * 0.24f, pawSize, scale * 0.24f),
        pawColor
    );
    model.addCube(
        position + glm::vec3(scale * 0.25f, scale * 0.08f, -scale * 0.25f),
        glm::vec3(scale * 0.24f, pawSize, scale * 0.24f),
        pawColor
    );
    
    // 尾巴 - 使用更粗的连续段
    float tailWidth = scale * 0.12f;
    int tailSegments = 5;
    
    for (int i = 0; i < tailSegments; i++) {
        float progress = i / (float)(tailSegments - 1);
        float height = scale * (0.5f + progress * 0.8f);
        float zOffset = -scale * (0.4f + progress * 0.5f);
        float xOffset = scale * sin(progress * 3.14159f) * 0.2f;
        
        // 主段
        model.addCube(
            position + glm::vec3(xOffset, height, zOffset),
            glm::vec3(tailWidth, tailWidth, scale * 0.18f),
            glm::mix(mainColor, darkColor, progress * 0.6f)
        );
        
        // 连接填充
        if (i < tailSegments - 1) {
            float nextProgress = (i + 1) / (float)(tailSegments - 1);
            float nextHeight = scale * (0.5f + nextProgress * 0.8f);
            float nextZOffset = -scale * (0.4f + nextProgress * 0.5f);
            float nextXOffset = scale * sin(nextProgress * 3.14159f) * 0.2f;
            
            model.addCube(
                position + glm::vec3((xOffset + nextXOffset) * 0.5f, (height + nextHeight) * 0.5f, (zOffset + nextZOffset) * 0.5f),
                glm::vec3(tailWidth * 0.8f, tailWidth * 0.8f, scale * 0.1f),
                glm::mix(mainColor, darkColor, (progress + nextProgress) * 0.3f)
            );
        }
    }
    
    model.setupMesh();
    return model;
}

Model Model::createRabbit(const glm::vec3& position, float scale) {
    Model model;
    
    // 身体 - 主体
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, 0.0f),
        glm::vec3(scale * 0.5f, scale * 0.4f, scale * 0.6f),
        glm::vec3(1.0f, 1.0f, 1.0f)  // 白色
    );
    
    // 头部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.9f, scale * 0.3f),
        glm::vec3(scale * 0.4f, scale * 0.4f, scale * 0.4f),
        glm::vec3(1.0f, 1.0f, 1.0f)  // 白色
    );
    
    // 耳朵
    model.addCube(  // 左耳
        position + glm::vec3(-scale * 0.15f, scale * 1.4f, scale * 0.3f),
        glm::vec3(scale * 0.1f, scale * 0.4f, scale * 0.1f),
        glm::vec3(0.95f, 0.9f, 0.9f)  // 浅粉色
    );
    model.addCube(  // 右耳
        position + glm::vec3(scale * 0.15f, scale * 1.4f, scale * 0.3f),
        glm::vec3(scale * 0.1f, scale * 0.4f, scale * 0.1f),
        glm::vec3(0.95f, 0.9f, 0.9f)  // 浅粉色
    );
    
    // 腿部
    float legHeight = scale * 0.3f;
    float legWidth = scale * 0.15f;
    // 前
    model.addCube(  // 左前腿
        position + glm::vec3(-scale * 0.2f, legHeight * 0.5f, scale * 0.2f),
        glm::vec3(legWidth, legHeight, legWidth),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    model.addCube(  // 前腿
        position + glm::vec3(scale * 0.2f, legHeight * 0.5f, scale * 0.2f),
        glm::vec3(legWidth, legHeight, legWidth),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    // 后腿（更大更长）
    model.addCube(  // 左后腿
        position + glm::vec3(-scale * 0.2f, legHeight * 0.7f, -scale * 0.2f),
        glm::vec3(legWidth * 1.2f, legHeight * 1.4f, legWidth * 1.2f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    model.addCube(  // 右后腿
        position + glm::vec3(scale * 0.2f, legHeight * 0.7f, -scale * 0.2f),
        glm::vec3(legWidth * 1.2f, legHeight * 1.4f, legWidth * 1.2f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    );
    
    // 尾巴（圆形）
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, -scale * 0.4f),
        glm::vec3(scale * 0.2f, scale * 0.2f, scale * 0.1f),
        glm::vec3(1.0f, 1.0f, 1.0f)  // 白色
    );
    
    model.setupMesh();
    return model;
}

Model Model::createBird(const glm::vec3& position, float scale) {
    Model model;
    
    // 身体 - 主体
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, 0.0f),
        glm::vec3(scale * 0.4f, scale * 0.4f, scale * 0.6f),
        glm::vec3(0.2f, 0.6f, 1.0f)  // 蓝色
    );
    
    // 头部
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.8f, scale * 0.4f),
        glm::vec3(scale * 0.3f, scale * 0.3f, scale * 0.3f),
        glm::vec3(0.2f, 0.6f, 1.0f)  // 蓝色
    );
    
    // 喙
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.75f, scale * 0.6f),
        glm::vec3(scale * 0.1f, scale * 0.1f, scale * 0.2f),
        glm::vec3(1.0f, 0.8f, 0.0f)  // 黄色
    );
    
    // 翅膀
    model.addCube(  // 左翅膀
        position + glm::vec3(-scale * 0.4f, scale * 0.6f, 0.0f),
        glm::vec3(scale * 0.1f, scale * 0.2f, scale * 0.4f),
        glm::vec3(0.1f, 0.5f, 0.9f)  // 深蓝色
    );
    model.addCube(  // 右翅膀
        position + glm::vec3(scale * 0.4f, scale * 0.6f, 0.0f),
        glm::vec3(scale * 0.1f, scale * 0.2f, scale * 0.4f),
        glm::vec3(0.1f, 0.5f, 0.9f)  // 深蓝色
    );
    
    // 尾巴
    model.addCube(
        position + glm::vec3(0.0f, scale * 0.5f, -scale * 0.4f),
        glm::vec3(scale * 0.3f, scale * 0.1f, scale * 0.2f),
        glm::vec3(0.1f, 0.5f, 0.9f)  // 深蓝色
    );
    
    // 腿
    float legHeight = scale * 0.2f;
    float legWidth = scale * 0.05f;
    model.addCube(  // 左腿
        position + glm::vec3(-scale * 0.1f, legHeight * 0.5f, 0.0f),
        glm::vec3(legWidth, legHeight, legWidth),
        glm::vec3(1.0f, 0.8f, 0.0f)  // 黄色
    );
    model.addCube(  // 右腿
        position + glm::vec3(scale * 0.1f, legHeight * 0.5f, 0.0f),
        glm::vec3(legWidth, legHeight, legWidth),
        glm::vec3(1.0f, 0.8f, 0.0f)  // 黄色
    );
    
    model.setupMesh();
    return model;
} 