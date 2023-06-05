#version 420 core

const float n1divn2 = 0.75f;
const float n2divn1 = 1.0f / n1divn2;
const float F0 = 0.17f; //1.0f / 49.0f; // [(4/3-1)/(4/3+1)]^(2/2.2)

struct light_t {
	vec4 position;
	vec4 color;
	vec4 ambient;
	vec4 k;
};

in vec3 normal;
in vec3 localPos;
in vec4 worldPos;
in vec2 texCoord;
layout(location = 0) out vec4 outColor;

uniform sampler2D wavesTex;
uniform samplerCube cubeTex;

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

vec3 intersectRay(vec3 p, vec3 vec)
{
	vec3 tv = max((-p + vec3(0.5f, 0.5f, 0.5f)) / vec, (-p - vec3(0.5f, 0.5f, 0.5f)) / vec);
	float t = min(tv.x, min(tv.y, tv.z));
	return p + t * vec;
}

float fresnel(float cos)
{
	return F0 + (1.0f - F0) * pow(1.0f - cos, 5.0f);
}

void main()
{
	vec4 disturbed = texture(wavesTex, texCoord);
	vec3 normal2 = normalMapping(disturbed);
	vec3 toLight = normalize(light.position.xyz - worldPos.xyz);
	vec3 toCamera = normalize(vec3(-camPos.x,-camPos.y,-camPos.z) - worldPos.xyz);
	vec3 reflected = reflect(-toCamera, normal2);

	float n = n1divn2;

	float cos = dot(normal2, toCamera);
	if (cos < 0)
	{
		n = n2divn1;
		normal2 = -normal2;
		cos = -cos;
	}
	vec3 refracted = refract(-toCamera, normal2, n);

	vec4 reflectedColor = texture(cubeTex, intersectRay(localPos, reflected));
	vec4 refractedColor = texture(cubeTex, intersectRay(localPos, refracted));

	if (length(refracted) > 0)
	{
		float t = fresnel(cos);
		outColor = refractedColor * (1 - t) + reflectedColor * t;
	}
	else
	{
		outColor = reflectedColor;
	}

	//outColor = vec4(toCamera, 1.0f);
	//outColor = vec4(reflected, 1.0f);
	//outColor = light.k.x * light.ambient * surfColor + light.k.y * surfColor * light.color * dot(toLight, normal2) + light.k.z * light.color * pow(dot(toCamera, reflected), light.k.w);
}