#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 Color;
in vec4 FragPosLightSpace;

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
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace) {
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 获取最近的深度值
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 获取当前片段的深度
    float currentDepth = projCoords.z;
    // 检查是否在阴影中
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, float shadow) {
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

    // 应用阴影
    return ambient + (1.0 - shadow) * (diffuse + specular);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // 计算阴影
    float shadow = ShadowCalculation(FragPosLightSpace);
    
    vec3 result = vec3(0.0);
    
    // 计算所有光源的贡献
    for(int i = 0; i < numLights; i++) {
        result += CalcLight(lights[i], norm, FragPos, viewDir, shadow);
    }
    
    // 应用颜色
    result *= Color;
    
    // 输出最终颜色
    FragOutput = vec4(result, 1.0);
} 