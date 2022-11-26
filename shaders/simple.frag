#version 460

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec3 fragPosWorld;
layout (location = 2) in vec3 fragNormalWorld;

layout(location = 0) out vec4 outColor;

struct Light {
    vec3 lightPosition;
    vec4 ambientColor;
    vec4 lightColor;
};

layout (set = 0, binding = 0) uniform GlobalUniformBuffer {
    mat4 projectionMatrix;
    mat4 viewMatrix;
    
	Light light; 
} ubo;

layout (push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;


void main() {
    vec3 directionToLight = ubo.light.lightPosition - fragPosWorld;
    float attenuation = 1.0f / dot(directionToLight, directionToLight);

    vec3 lightColor = ubo.light.lightColor.xyz * ubo.light.lightColor.w * attenuation;
    vec3 ambientLight = ubo.light.ambientColor.xyz * ubo.light.ambientColor.w;
    vec3 diffuseLight = lightColor * max(dot(normalize(fragNormalWorld), normalize(directionToLight)), 0);
    outColor = vec4((diffuseLight + ambientLight) * fragColor, 1.0);
//    outColor = vec4(1.0f, 0.0f, 0.0f, 0.0f);
}

