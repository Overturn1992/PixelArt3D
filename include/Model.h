#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
};

class Model {
public:
    std::vector<Vertex> vertices;
    unsigned int VAO, VBO;

    Model();
    ~Model();

    void setupMesh();
    void draw() const;

    // 静态方法用于创建基本形状
    static Model createCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
    static Model createGround(float width, float depth, const glm::vec3& color);
    
    // 动物模型创建函数
    static Model createCat(const glm::vec3& position, float scale);
    static Model createRabbit(const glm::vec3& position, float scale);
    static Model createBird(const glm::vec3& position, float scale);

private:
    void addCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3& color);
};

#endif // MODEL_H 