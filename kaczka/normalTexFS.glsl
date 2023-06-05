#version 420 core

const float N = 256.0f;

in vec2 texCoord;
layout(location = 0) out vec4 outColor;

uniform sampler2D wavesTex;

void main()
{
	float step = 1 / N;

	vec4 znipj = texture(wavesTex, vec2(texCoord.x + step, texCoord.y)),
		znimj = texture(wavesTex, vec2(texCoord.x - step, texCoord.y)),
		znijp = texture(wavesTex, vec2(texCoord.x, texCoord.y + step)),
		znijm = texture(wavesTex, vec2(texCoord.x, texCoord.y - step));
	
	vec3 grad = normalize(vec3((znipj.x - znimj.x) * N * 0.5f, (znijp.x - znijm.x) * N * 0.5f, 1.0f));

	//outColor = 0.25*(znipj + znimj + znijp + znijm) + znij-znmij;
	//outColor *= 10;

	outColor = vec4(0.5f * (grad + vec3(1.0f, 1.0f, 1.0f)), 1.0f);

	//outColor = vec4(A, 0.0f, 0.0f, 1.0f);
}
