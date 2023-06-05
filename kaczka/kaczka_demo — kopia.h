#pragma once

#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "frame_buffer.h"

namespace kaczka
{
	struct Light {
		glm::vec3 position;
		glm::vec4 color;
		glm::vec4 ambient;
		glm::vec3 k;
	};

	class KaczkaDemo
	{
		static const Light light;

		static const int N = 256;
		const Camera& camera;

		Renderer renderer;

		Mesh water;

		Texture waves;
		Texture prevWaves;
		Texture waveNormals;
		bool writeToPrev = false;
		FrameBuffer fbo, prevFbo, normalFbo;

		Shader phongShader;
		Shader waterTexShader;
		Shader normalTexShader;

		UniformBuffer<glm::mat4> matricesUbo;
		UniformBuffer<Light> lightUbo;
		UniformBuffer<glm::vec4> meshPropUbo;
		UniformBuffer<glm::vec3> cameraUbo;
		UniformBuffer<glm::vec2> dropCoordUbo;

		void renderWater();
		void updateWaterTextures();
	public:
		KaczkaDemo(const Camera& camera);
		void init();
		void update();
		void render();
	};
}