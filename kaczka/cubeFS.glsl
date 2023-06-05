#version 420 core

in vec3 texCoord;
layout(location = 0) out vec4 outColor;

uniform samplerCube cubeTex;

void main()
{
	outColor = texture(cubeTex, texCoord);
}