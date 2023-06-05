#version 420 core

const float EPS = 0.0001f;
const float PI = 3.14159265358979323846f;

struct light_t {
	vec4 position;
	vec4 color;
	vec4 ambient;
	vec4 k;
};

layout(std140, binding = 1) uniform Light
{
	light_t light;
};

layout(std140, binding = 2) uniform MeshProperties
{
	float roughness;
	float isotropy;
};

layout(std140, binding = 3) uniform Camera
{
	vec3 camPos;
};

in vec3 normal;
in vec4 worldPos;
in vec2 texCoord;
layout(location = 0) out vec4 outColor;

uniform sampler2D duckTex;

vec3 radiance(vec3 n, vec3 l, float dist)
{
	return light.color * max(dot(n, l), 0) / (dist * dist);
}

vec3 spectralFactor(vec3 albedo, float u)
{
	return albedo + (1.0f - albedo) * pow(1 - u, 5);
}

float Z(float t)
{
	float denominator = 1.0f + (roughness - 1.0f) * t * t;
	return roughness / (denominator * denominator);
}

float A(float w)
{
	return sqrt(isotropy / ((w * w) + isotropy * isotropy * (1 - w * w)));
}

float directionalFactor(float t, float v, float vp, float w)
{
	//if (v < EPS || vp < EPS) return 0.0f;
	return 0.25f / (PI * max(v, EPS) * max(vp, EPS)) * Z(t) * A(w);
}

// https://wiki.jmonkeyengine.org/docs/3.3/tutorials/_attachments/Schlick94.pdf
vec3 schlickBRDFSingle(vec3 albedo, float t, float u, float v, float vp, float w)
{
	return spectralFactor(albedo, u) * directionalFactor(t, v, vp, w);
}

vec3 halfVector(vec3 v1, vec3 v2)
{
	return normalize(v1 + v2);
}

vec3 gammaDecode(vec3 color)
{
	return pow(color, vec3(2.2f, 2.2f, 2.2f));
}

vec3 gammaEncode(vec3 color)
{
	return pow(color, vec3(0.4545f, 0.4545f, 0.4545f));
}

void main()
{
	vec3 texColor = texture(duckTex, texCoord).xyz;
	vec3 albedo = gammaDecode(texColor);
	vec3 toLight = (light.position - worldPos).xyz;
	float toLightDist = length(toLight);
	vec3 toCamera = normalize(-camPos - worldPos.xyz);
	toLight /= toLightDist; // normalizes toLight

	vec3 h = halfVector(toCamera, toLight);

	vec3 tangent = cross(normal, vec3(0.0f, 1.0f, 0.0f));
	if (length(tangent) < EPS)
		tangent = vec3(1.0f, 0.0f, 0.0f);
	else
		tangent = normalize(tangent);
	vec3 binormal = normalize(cross(normal, tangent));

	vec3 hProj = normalize(h - normal * dot(h, normal));

	float t = max(dot(h, normal), 0),
		u = max(dot(h, toCamera), 0),
		v = dot(toCamera, normal),
		vp = dot(toLight, normal),
		w = dot(binormal, hProj);

	//outColor = vec4(gammaEncode(schlickBRDFSingle(albedo, t, u, v, vp, w) * radiance(normal, toLight, toLightDist)), 1.0f);
	outColor = vec4(gammaEncode(schlickBRDFSingle(albedo, t, u, v, vp, w)) * radiance(normal, toLight, toLightDist) + light.k.x * light.ambient.xyz * texColor, 1.0f);
}