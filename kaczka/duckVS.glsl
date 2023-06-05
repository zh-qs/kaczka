#version 420 core

layout(location = 0) in vec3 inputPos;
layout(location = 1) in vec3 inputNormal;
layout(location = 2) in vec2 inputTexCoord;

out vec3 normal;
out vec4 worldPos;
out vec2 texCoord;

layout(std140, binding = 0) uniform Matrices
{
	mat4 model;
	mat4 pv;
};

void main()
{
	worldPos = model * vec4(inputPos, 1.0f);
	normal = normalize((model * vec4(inputNormal, 0.0f)).xyz);
	//if (length(normal) < 1.0f)
	//	normal = vec3(1.0f, 0.0f, 0.0f); // XD
	texCoord = inputTexCoord;
	gl_Position = pv * worldPos;
}
