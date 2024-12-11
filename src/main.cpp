#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "Material.h"

// 相机
Camera camera(15.0f);

// 时间
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 错误回调函数
void errorCallback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

// OpenGL调试回调
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") 
              << " type = " << type << ", severity = " << severity 
              << ", message = " << message << std::endl;
}

// 处理键盘输入
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.RotateLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.RotateRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.RotateUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.RotateDown(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
        camera.ZoomIn(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
        camera.ZoomOut(deltaTime);
}

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 设置OpenGL版本和核心模式
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Pixel Animals", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 设置当前上下文
    glfwMakeContextCurrent(window);

    // 初始化GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // 启用OpenGL调试输出
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(messageCallback, 0);

    // 打印OpenGL版本信息
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // 创建并编译着色器
    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    std::cout << "Shader program created with ID: " << shader.ID << std::endl;

    // 创建地面
    Model ground = Model::createGround(10.0f, 10.0f, glm::vec3(0.4f, 0.8f, 0.4f));
    
    // 创建猫模型
    Model cat = Model::createCat(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f);
    std::cout << "Cat model created with " << cat.vertices.size() << " vertices" << std::endl;

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        // 计算帧时间
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // 处理输入
        processInput(window);

        // 清除颜色缓冲和深度缓冲
        glClearColor(0.7f, 0.9f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 设置视口
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // 激活着色器
        shader.use();

        // 创建变换矩阵
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // 设置变换矩阵
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // 设置相机位置
        shader.setVec3("viewPos", camera.Position);

        // 设置光源
        shader.setInt("numLights", 1);
        shader.setVec3("lights[0].position", glm::vec3(5.0f, 8.0f, 5.0f));
        shader.setVec3("lights[0].ambient", glm::vec3(0.3f));
        shader.setVec3("lights[0].diffuse", glm::vec3(1.0f));
        shader.setVec3("lights[0].specular", glm::vec3(1.0f));
        shader.setFloat("lights[0].constant", 1.0f);
        shader.setFloat("lights[0].linear", 0.014f);
        shader.setFloat("lights[0].quadratic", 0.0007f);

        // 绘制地面
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(0.0f, -0.5f, 0.0f));
        shader.setMat4("model", groundModel);
        
        // 设置地面材质
        shader.setVec3("material.ambient", glm::vec3(0.2f));
        shader.setVec3("material.diffuse", glm::vec3(0.8f));
        shader.setVec3("material.specular", glm::vec3(0.2f));
        shader.setFloat("material.shininess", 16.0f);
        
        ground.draw();

        // 绘制猫
        glm::mat4 catModel = glm::mat4(1.0f);
        catModel = glm::translate(catModel, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", catModel);
        
        // 设置猫的材质
        shader.setVec3("material.ambient", glm::vec3(0.3f));
        shader.setVec3("material.diffuse", glm::vec3(0.8f));
        shader.setVec3("material.specular", glm::vec3(0.5f));
        shader.setFloat("material.shininess", 32.0f);
        
        cat.draw();

        // 检查OpenGL错误
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }

        // 交换缓冲并处理事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 清理资源
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
} 