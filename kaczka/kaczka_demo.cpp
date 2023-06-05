#include "kaczka_demo.h"

#include <random>
#include <bmpmini.hpp>
#include <glm/gtx/transform.hpp>

namespace kaczka
{
	const Light KaczkaDemo::light =
	{
		{ 0.0f,2.0f,0.0f,1.0f },
		{ 5.0f,5.0f,5.0f,1.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 0.2f,0.5f,0.4f,30.0f },
	};

	KaczkaDemo::KaczkaDemo(const Camera& camera) : camera(camera) {}

	void KaczkaDemo::init()
	{
		renderer.init();

		// meshes
		water = Mesh::squareGridXY(2);
		cube = Mesh::invertedCube();
		duck = Mesh::loadModel("models/duck.txt");
		duck.model = glm::scale(glm::vec3{ DUCK_SCALE,DUCK_SCALE,DUCK_SCALE });
		duck.color = { 0.8f, 0.1f, 0.0f, 0.0f }; // roughness and isotropy

		// shaders
		waterShader.init("waterVS.glsl", "waterFS.glsl");
		waterShader.use();
		glUniform1i(waterShader.get_uniform_location("wavesTex"), 0);
		glUniform1i(waterShader.get_uniform_location("cubeTex"), 1);
		cubeShader.init("cubeVS.glsl", "cubeFS.glsl");
		cubeShader.use();
		glUniform1i(cubeShader.get_uniform_location("cubeTex"), 0);
		duckShader.init("duckVS.glsl", "duckFS.glsl");
		duckShader.use();
		glUniform1i(duckShader.get_uniform_location("duckTex"), 0);

		// textures
		waveNormals.init();
		waveNormals.bind();
		waveNormals.configure();
		waveNormals.set_wrap_mode(GL_CLAMP_TO_BORDER);
		waveNormals.set_size(N, N);

		cubeTex.init();
		cubeTex.bind();
		cubeTex.configure();
		loadCubeTexture();

		duckTex.init();
		duckTex.bind();
		duckTex.configure();
		loadDuckTexture();

		// UBOs
		matricesUbo.init(2);
		lightUbo.init(1);
		meshPropUbo.init(1);
		cameraUbo.init(1);

		renderer.addUniformBlock(matricesUbo);
		renderer.addUniformBlock(lightUbo);
		renderer.addUniformBlock(meshPropUbo);
		renderer.addUniformBlock(cameraUbo);

		// set uniform data
		lightUbo.set_data(&light);
	}

	void KaczkaDemo::update(float dt)
	{
		auto pos = camera.getPosition();
		cameraUbo.set_data(&pos);
		glm::mat4 pv = camera.getProjMatrix() * camera.getViewMatrix();
		matricesUbo.set_data(&pv, 1);
		updateWaterTextures();
		updateDuckPosition(dt);
	}

	void KaczkaDemo::renderWater()
	{
		matricesUbo.set_data(&water.model);
		meshPropUbo.set_data(&water.color);
		glDisable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		waveNormals.bind();
		glActiveTexture(GL_TEXTURE1);
		cubeTex.bind();
		renderer.render(water, waterShader);
		glEnable(GL_CULL_FACE);
	}

	void KaczkaDemo::renderCube()
	{
		matricesUbo.set_data(&cube.model);
		glActiveTexture(GL_TEXTURE0);
		cubeTex.bind();
		renderer.render(cube, cubeShader);
	}

	void KaczkaDemo::renderDuck()
	{
		matricesUbo.set_data(&duck.model);
		meshPropUbo.set_data(&duck.color);
		glActiveTexture(GL_TEXTURE0);
		duckTex.bind();
		renderer.render(duck, duckShader);
	}

	void KaczkaDemo::updateWaterTextures()
	{
		static std::random_device dev;
		static std::uniform_real_distribution<float> random;

		float prob = random(dev);
		if (prob < 0.1f)
			simulation.disturb(static_cast<int>(std::roundf(N * random(dev))), static_cast<int>(std::roundf(N * random(dev))));

		simulation.propagate();
		simulation.computeNormals();
		waveNormals.bind();
		waveNormals.set_size(N, N, simulation.normals);
	}

	void KaczkaDemo::updateDuckPosition(float dt)
	{
		glm::vec3 tangent;
		auto newPosition = duckPath.next(dt, tangent);
		tangent = -glm::normalize(tangent);
		glm::vec3 normal = { 0.0f,1.0f,0.0f };
		glm::vec3 binormal = glm::normalize(glm::cross(tangent, normal));

		glm::mat4 baseChange{ 1.0f };
		baseChange[0] = glm::vec4(tangent, 0.0f);
		baseChange[1] = glm::vec4(normal, 0.0f);
		baseChange[2] = glm::vec4(binormal, 0.0f);
		duck.model = glm::translate(newPosition - glm::vec3(0.5f, 0.0f, 0.5f)) * baseChange * glm::scale(glm::vec3{ DUCK_SCALE,DUCK_SCALE,DUCK_SCALE });
		simulation.disturb(static_cast<int>(std::roundf(N * newPosition.z)), static_cast<int>(std::roundf(N * newPosition.x)));
	}

	void KaczkaDemo::loadCubeTexture()
	{
		const char* filenames[] = {
			"textures/cubemap/0_right.bmp",
			"textures/cubemap/1_left.bmp",
			"textures/cubemap/2_top.bmp",
			"textures/cubemap/3_bottom.bmp",
			"textures/cubemap/4_back.bmp",
			"textures/cubemap/5_front.bmp"
		};
		for (int f = 0; f < 6; ++f)
		{
			image::BMPMini bmp;
			bmp.read(filenames[f]);
			auto img = bmp.get();
			std::vector<glm::vec4> imageVec(img.width * img.height);
			for (int i = 0; i < imageVec.size(); ++i)
			{
				imageVec[i] = { img.data[3 * i + 2] / 255.0f,img.data[3 * i + 1] / 255.0f,img.data[3 * i] / 255.0f,1.0f };
			}
			cubeTex.set_size(f, img.width, img.height, imageVec.data());
		}
	}

	void KaczkaDemo::loadDuckTexture()
	{
		image::BMPMini bmp;
		bmp.read("textures/ducktex.bmp");
		auto img = bmp.get();
		std::vector<glm::vec4> imageVec(img.width * img.height);
		for (int i = 0; i < imageVec.size(); ++i)
		{
			imageVec[i] = { img.data[3 * i + 2] / 255.0f, img.data[3 * i + 1] / 255.0f, img.data[3 * i] / 255.0f, 1.0f };
		}
		duckTex.set_size(img.width, img.height, imageVec.data());
	}

	void KaczkaDemo::render()
	{
		renderWater();
		renderCube();
		renderDuck();
	}
}

