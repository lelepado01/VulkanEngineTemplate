
#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec3 fragPosWorld;
layout (location = 2) out vec3 fragNormalWorld;

layout (set = 0, binding = 0) uniform GlobalUniformBuffer {
    mat4 projectionMatrix;
    mat4 viewMatrix;

	vec3 lightPosition;
    vec3 ambientColor;
	vec3 lightColor;    
} ubo;

layout (push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    gl_Position = ubo.projectionMatrix * ubo.viewMatrix * vec4(position, 1.0f);
    
    fragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
    fragPosWorld = position;
    fragColor = vec3(1,0.5,1); //color;
}
