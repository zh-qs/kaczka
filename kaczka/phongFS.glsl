#version 420 core

struct light_t {
	vec4 position;
	vec4 color;
	vec4 ambient;
	vec4 k;
};

in vec3 normal;
in vec4 worldPos;
in vec2 texCoord;
layout(location = 0) out vec4 outColor;

uniform sampler2D wavesTex;

layout(std140, binding = 1) uniform Light 
{
	light_t light;
};

layout(std140, binding = 2) uniform MeshProperties
{
	vec4 surfColor;
};

layout(std140, binding = 3) uniform Camera
{
	vec3 camPos;
};

vec3 normalMapping(vec4 disturbed)
{
	disturbed = 2.0f * disturbed - 1.0f;
	return normalize(disturbed.xzy);
}

void main()
{
	vec4 disturbed = texture(wavesTex, texCoord);
	vec3 normal2 = normalMapping(disturbed);
	vec3 toLight = normalize(light.position.xyz - worldPos.xyz);
	vec3 toCamera = normalize(camPos - worldPos.xyz);
	vec3 reflected = normalize(reflect(toLight, normal2));

	outColor = light.k.x * light.ambient * surfColor + light.k.y * surfColor * light.color * dot(toLight, normal2) + light.k.z * light.color * pow(dot(toCamera, reflected), light.k.w);
	//outColor = vec4(light.k.x,0.0f,0.0f,1.0f);
}