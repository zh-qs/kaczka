#version 420 core

layout(location = 0) in vec3 inputPos;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(2.0f * inputPos.x, 2.0f * inputPos.z, 0.0f, 1.0f);
	texCoord = vec2(inputPos.x + 0.5f, inputPos.z + 0.5f);
}
