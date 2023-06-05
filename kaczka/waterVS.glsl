#version 420 core

layout(location = 0) in vec3 inputPos;
layout(location = 1) in vec3 inputNormal;

out vec3 normal;
out vec3 localPos;
out vec4 worldPos;
out vec2 texCoord;

layout(std140, binding = 0) uniform Matrices
{
	mat4 model;
	mat4 pv;
};

void main() 
{
	localPos = inputPos;
	worldPos = model * vec4(inputPos, 1.0f);
	normal = (model * vec4(inputNormal, 0.0f)).xyz;
	texCoord = vec2(inputPos.x + 0.5f, inputPos.z + 0.5f);
	gl_Position = pv * worldPos;
}
