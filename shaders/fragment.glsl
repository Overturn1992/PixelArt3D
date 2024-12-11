#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;

out vec4 FragOutput;

#define MAX_LIGHTS 4

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    // 计算光照方向和距离
    vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // 环境光
    vec3 ambient = light.ambient * material.ambient;

    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // 应用衰减
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    
    // 计算所有光源的贡献
    for(int i = 0; i < numLights; i++) {
        result += CalcLight(lights[i], norm, FragPos, viewDir);
    }
    
    // 应用颜色
    result *= Color;
    
    // 输出最终颜色
    FragOutput = vec4(result, 1.0);
} 