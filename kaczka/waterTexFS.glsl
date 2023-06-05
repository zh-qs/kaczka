#version 420 core

const float N = 256.0f;
const float c = 1.0f;
const float A = 0.25f * c * c * (N - 1) * (N - 1) / (N * N);
const float B = 2.0f - 4.0f * A;

in vec2 texCoord;
layout(location = 0) out vec4 outColor;

uniform sampler2D waves;
uniform sampler2D prevWaves;

layout(std140, binding = 4) uniform Drop
{
	vec2 dropCoord;
};

void main()
{
	float l = 2.0f * min(0.5f - abs(texCoord.x - 0.5f), 0.5f - abs(texCoord.y - 0.5f));
	float dij = 0.95f * min(1.0f, l * 5.0f);

	float step = 1 / N;

	vec4 znipj = texture(waves, vec2(texCoord.x + step, texCoord.y)),
		znimj = texture(waves, vec2(texCoord.x - step, texCoord.y)),
		znijp = texture(waves, vec2(texCoord.x, texCoord.y + step)),
		znijm = texture(waves, vec2(texCoord.x, texCoord.y - step)),
		znij = texture(waves, texCoord),
		znmij = texture(prevWaves, texCoord);
	outColor = dij * (A * (znipj + znimj + znijp + znijm) + B * znij - znmij);

	if (dropCoord.x >= 0 && dropCoord.y >= 0 && length(dropCoord - texCoord) < 0.5f/N)
		outColor += 1.0f * vec4(1.0f, 1.0f, 1.0f, 0.0f);

	outColor.w = 1.0f;
}
