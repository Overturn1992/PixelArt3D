#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // 计算世界空间位置
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = vec3(worldPos);
    
    // 计算法线
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // 传递颜色
    Color = aColor;
    
    // 计算裁剪空间位置
    gl_Position = projection * view * worldPos;
} 