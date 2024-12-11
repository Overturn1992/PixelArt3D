#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform vec2 pixelSize; // 像素大小

void main() {
    // 计算像素化后的纹理坐标
    vec2 pixelCoord = vec2(
        floor(TexCoords.x / pixelSize.x) * pixelSize.x + pixelSize.x * 0.5,
        floor(TexCoords.y / pixelSize.y) * pixelSize.y + pixelSize.y * 0.5
    );
    
    // 采样像素化后的颜色
    vec4 color = texture(screenTexture, pixelCoord);
    
    // 输出最终颜色
    FragColor = color;
} 