# 3D像素风格建模程序代码文档

## 一、主程序 (main.cpp)

### 1. 程序入口与初始化
```cpp
int main() {
    // 初始化GLFW和OpenGL上下文
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PixelArt3D", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    // 初始化GLEW
    glewInit();
}
```

实现思路：
1. 使用GLFW创建OpenGL上下文和窗口
   - 设置OpenGL版本为3.3
   - 使用核心模式以获取现代OpenGL特性
   - 创建固定大小的窗口
2. 初始化GLEW以访问OpenGL函数
3. 设置视口大小和基本OpenGL状态

### 2. 渲染循环设计
```cpp
void renderLoop() {
    // 时间管理
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    // 清除缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // 更新场景
    updateScene(deltaTime);
    
    // 渲染过程
    renderShadowMap();     // 第一遍渲染：生成阴影贴图
    renderScene();         // 第二遍渲染：主场景
    applyPostProcessing(); // 第三遍渲染：后处理
}
```

实现思路：
1. 时间管理
   - 计算帧间时间以实现平滑动画
   - 用于相机移动和动画更新
2. 多遍渲染技术
   - 第一遍：从光源视角渲染深度图
   - 第二遍：使用深度图渲染带阴影的场景
   - 第三遍：应用像素化后处理效果

## 二、着色器系统详解

### 1. 着色器管理器设计
```cpp
class Shader {
private:
    // 编译和错误检查
    void checkCompileErrors(GLuint shader, std::string type) {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " 
                         << type << "\n" << infoLog << std::endl;
            }
        } else {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " 
                         << type << "\n" << infoLog << std::endl;
            }
        }
    }
};
```

实现思路：
1. 着色器加载和编译
   - 从文件读取着色器源码
   - 分别编译顶点和片段着色器
   - 链接着色器程序
2. 错误处理机制
   - 检查编译错误
   - 检查链接错误
   - 提供详细的错误信息
3. Uniform变量管理
   - 缓存uniform位置以提高性能
   - 提供类型安全的设置接口

### 2. 着色器程序实现
```cpp
// 顶点着色器
void main() {
    // 计算世界空间位置
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // 计算法线（考虑非统一缩放）
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // 计算光空间位置（用于阴影映射）
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    
    // 最终位置
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
```

实现思路：
1. 坐标变换流程
   - 模型空间 → 世界空间
   - 世界空间 → 观察空间
   - 观察空间 → 裁剪空间
2. 法线变换
   - 使用法线矩阵处理非统一缩放
3. 阴影映射准备
   - 计算光源视角下的顶点位置

## 三、相机系统实现

### 1. 轨道相机设计
```cpp
class Camera {
private:
    // 相机状态
    glm::vec3 Position;
    glm::vec3 Target;     // 观察目标点
    float Distance;       // 到目标点的距离
    float Yaw, Pitch;    // 欧拉角
    
    // 更新相机向量
    void updateCameraVectors() {
        // 根据欧拉角计算前向量
        Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front.y = sin(glm::radians(Pitch));
        Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(Front);
        
        // 重新计算右向量和上向量
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
```

实现思路：
1. 轨道相机模型
   - 维护目标点和距离
   - 使用球坐标系计算位置
2. 相机控制
   - A/D键：水平旋转（改变Yaw角）
   - W/S键：垂直旋转（改变Pitch角）
   - +/-键：缩放（改变Distance）
3. 平滑过渡
   - 使用线性插值实现平滑移动
   - 添加加速度和阻尼效果

### 2. 视图矩阵计算
```cpp
glm::mat4 Camera::GetViewMatrix() {
    // 计算实际相机位置
    Position = Target - Front * Distance;
    
    // 使用lookAt矩阵
    return glm::lookAt(Position, Target, Up);
}
```

实现思路：
1. 位置计算
   - 基于目标点和距离计算相机位置
   - 考虑边界情况（如距离限制）
2. 视图矩阵生成
   - 使用lookAt函数创建视图矩阵
   - 确保相机向量正交

## 四、模型系统实现

### 1. 方块生成系统
```cpp
class Model {
public:
    void createCube(glm::vec3 position, glm::vec3 size, Material material) {
        // 生成顶点数据
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
        // 为每个面生成四个顶点
        for (int face = 0; face < 6; face++) {
            // 计算面的四个顶点
            glm::vec3 normal = getFaceNormal(face);
            std::vector<glm::vec3> faceVertices = getFaceVertices(face, size);
            
            // 添加顶点
            for (const auto& pos : faceVertices) {
                Vertex vertex;
                vertex.Position = pos + position;
                vertex.Normal = normal;
                vertices.push_back(vertex);
            }
            
            // 添加索引（两个三角形）
            unsigned int baseIndex = face * 4;
            indices.push_back(baseIndex + 0);
            indices.push_back(baseIndex + 1);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 2);
            indices.push_back(baseIndex + 3);
            indices.push_back(baseIndex + 0);
        }
        
        // 设置材质
        this->material = material;
        
        // 更新GPU缓冲区
        updateBuffers(vertices, indices);
    }
};
```

实现思路：
1. 顶点生成
   - 为每个面生成四个顶点
   - 计算正确的法线方向
   - 应用位置和缩放变换
2. 索引组织
   - 使用索引绘制减少顶点数据
   - 每个面使用两个三角形
3. 材质系统
   - 支持不同的材质属性
   - 实现基于物理的渲染

### 2. 实例化渲染
```cpp
class Model {
private:
    // 实例化数据
    std::vector<glm::mat4> instanceMatrix;
    unsigned int instanceVBO;
    
public:
    void setupInstancing(const std::vector<glm::vec3>& positions) {
        // 生成变换矩阵
        for (const auto& pos : positions) {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
            instanceMatrix.push_back(model);
        }
        
        // 设置实例化缓冲区
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instanceMatrix.size() * sizeof(glm::mat4),
                    &instanceMatrix[0], GL_STATIC_DRAW);
                    
        // 设置实例化顶点属性
        for (unsigned int i = 0; i < 4; i++) {
            glEnableVertexAttribArray(3 + i);
            glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
                                (void*)(sizeof(glm::vec4) * i));
            glVertexAttribDivisor(3 + i, 1);
        }
    }
};
```

实现思路：
1. 实例化数据管理
   - 存储每个实例的变换矩阵
   - 使用实例化缓冲区
2. 性能优化
   - 减少Draw调用次数
   - 批量处理相同材质的模型

## 五、光照和阴影系统

### 1. 阴影映射实现
```cpp
class ShadowMapper {
private:
    unsigned int depthMapFBO;
    unsigned int depthMap;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    
public:
    void init() {
        // 创建帧缓冲对象
        glGenFramebuffers(1, &depthMapFBO);
        
        // 创建深度纹理
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                    SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT,
                    GL_FLOAT, NULL);
                    
        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        
        // 配置帧缓冲
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }
    
    void renderShadowMap(const Scene& scene, const Light& light) {
        // 配置光源视图矩阵和投影矩阵
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f,
                                              near_plane, far_plane);
        glm::mat4 lightView = glm::lookAt(light.position,
                                         glm::vec3(0.0f),
                                         glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;
        
        // 渲染深度图
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        
        // 使用阴影映射着色器
        shadowShader.use();
        shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        
        // 渲染场景
        scene.render(shadowShader);
    }
};
```

实现思路：
1. 深度图生成
   - 创建专用的帧缓冲和深度纹理
   - 从光源视角渲染场景
2. 阴影计算
   - 使用PCF（percentage-closer filtering）改善阴影质量
   - 处理阴影偏移避免阴影失真
3. 性能优化
   - 使用合适的阴影贴图分辨率
   - 实现级联阴影映射

### 2. 像素化效果实现
```cpp
class PixelateEffect {
private:
    unsigned int quadVAO, quadVBO;
    Shader pixelateShader;
    
public:
    void render(unsigned int sourceTexture, float pixelSize) {
        // 使用像素化着色器
        pixelateShader.use();
        pixelateShader.setInt("screenTexture", 0);
        pixelateShader.setFloat("pixelSize", pixelSize);
        
        // 绑定源纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sourceTexture);
        
        // 渲染全屏四边形
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
};
```

实现思路：
1. 后处理设计
   - 使用帧缓冲进行离屏渲染
   - 实现可调节的像素化效果
2. 像素化算法
   - 对纹理坐标进行量化
   - 实现可配置的像素大小
3. 性能考虑
   - 使用单次渲染通道
   - 优化着色器计算
</rewritten_file>