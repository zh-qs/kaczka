#pragma once

#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "frame_buffer.h"
#include "water_simulation.h"
#include "random_path.h"

namespace kaczka
{
	struct Light {
		glm::vec4 position;
		glm::vec4 color;
		glm::vec4 ambient;
		glm::vec4 k;
	};

	class KaczkaDemo
	{
		static const Light light;

		static constexpr int N = WaterSimulation::N;
		static constexpr float DUCK_SCALE = 0.001f;
		const Camera& camera;

		Renderer renderer;

		Mesh water;
		Mesh cube;
		Mesh duck;

		Texture2D waveNormals;
		TextureCUBE cubeTex;
		Texture2D duckTex;

		Shader waterShader;
		Shader cubeShader;
		Shader duckShader;

		UniformBuffer<glm::mat4> matricesUbo;
		UniformBuffer<Light> lightUbo;
		UniformBuffer<glm::vec4> meshPropUbo;
		UniformBuffer<glm::vec3> cameraUbo;

		WaterSimulation simulation;
		RandomPath duckPath;

		void renderWater();
		void renderCube();
		void renderDuck();
		void updateWaterTextures();
		void updateDuckPosition(float dt);
		void loadCubeTexture();
		void loadDuckTexture();
	public:
		KaczkaDemo(const Camera& camera);
		void init();
		void update(float dt);
		void render();
	};
}