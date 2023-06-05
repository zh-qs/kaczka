#version 420 core

layout(location = 0) in vec3 inputPos;

out vec3 texCoord;

layout(std140, binding = 0) uniform Matrices
{
	mat4 model;
	mat4 pv;
};

void main()
{
	texCoord = inputPos;
	gl_Position = pv * (model * vec4(inputPos, 1.0f));
}
