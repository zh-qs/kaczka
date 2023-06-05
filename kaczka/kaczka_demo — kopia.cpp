#include "kaczka_demo.h"

#include <random>

namespace kaczka
{
	const Light KaczkaDemo::light =
	{
		{ 0,2,0 },
		{ 1,1,1,1 },
		{ 1.0f,1.0f,1.0f,1.0f },
		{ 0.2f,0.5f,0.3f }
	};

	KaczkaDemo::KaczkaDemo(const Camera& camera) : camera(camera) {}

	void KaczkaDemo::init()
	{
		// meshes
		water = Mesh::squareGridXY(2);

		// shaders
		phongShader.init("phongVS.glsl", "phongFS.glsl");
		phongShader.use();
		glUniform1i(phongShader.get_uniform_location("wavesTex"), 0);
		waterTexShader.init("dummyVS.glsl", "waterTexFS.glsl");
		waterTexShader.use();
		glUniform1i(waterTexShader.get_uniform_location("waves"), 0);
		glUniform1i(waterTexShader.get_uniform_location("prevWaves"), 1);
		normalTexShader.init("dummyVS.glsl", "normalTexFS.glsl");
		normalTexShader.use();
		glUniform1i(normalTexShader.get_uniform_location("wavesTex"), 0);

		// textures
		fbo.init();
		fbo.bind();
		waves.init();
		waves.bind();
		waves.configure();
		waves.set_wrap_mode(GL_CLAMP_TO_BORDER);
		waves.set_size(N, N, GL_FLOAT);
		fbo.unbind();
		prevFbo.init();
		prevFbo.bind();
		prevWaves.init();
		prevWaves.bind();
		prevWaves.configure();
		prevWaves.set_wrap_mode(GL_CLAMP_TO_BORDER);
		prevWaves.set_size(N, N, GL_FLOAT);
		prevFbo.unbind();
		normalFbo.init();
		normalFbo.bind();
		waveNormals.init();
		waveNormals.bind();
		waveNormals.configure();
		waveNormals.set_wrap_mode(GL_CLAMP_TO_BORDER);
		waveNormals.set_size(N, N);
		normalFbo.unbind();

		// UBOs
		matricesUbo.init(2);
		lightUbo.init(1);
		meshPropUbo.init(1);
		cameraUbo.init(1);
		dropCoordUbo.init(1);

		renderer.addUniformBlock(matricesUbo);
		renderer.addUniformBlock(lightUbo);
		renderer.addUniformBlock(meshPropUbo);
		renderer.addUniformBlock(cameraUbo);
		renderer.addUniformBlock(dropCoordUbo);

		// set uniform data
		lightUbo.set_data(&light);
	}

	void KaczkaDemo::update()
	{
		cameraUbo.set_data(&camera.getPosition());
		glm::mat4 pv = camera.getProjMatrix() * camera.getViewMatrix();
		matricesUbo.set_data(&pv, 1);
		updateWaterTextures();
	}

	void KaczkaDemo::renderWater()
	{
		matricesUbo.set_data(&water.model);
		meshPropUbo.set_data(&water.color);
		glActiveTexture(GL_TEXTURE0);
		if (!writeToPrev)
			prevWaves.bind();
		else			
			waves.bind();
		//waveNormals.bind();
		renderer.render(water, phongShader);
	}

	void KaczkaDemo::updateWaterTextures()
	{
		static std::random_device dev;
		static std::uniform_real_distribution<float> random;

		glm::vec2 drop;
		float prob = random(dev);
		if (prob < 0.1f)
			drop = { random(dev), random(dev) };
		else
			drop = { -1.0f,-1.0f };
		dropCoordUbo.set_data(&drop);

		if (writeToPrev)
			prevFbo.bind();
		else
			fbo.bind();
		glViewport(0, 0, N, N);
		if (writeToPrev)
		{
			glActiveTexture(GL_TEXTURE0);
			waves.bind();
			glActiveTexture(GL_TEXTURE1);
			prevWaves.bind();
		}
		else
		{
			glActiveTexture(GL_TEXTURE0);
			prevWaves.bind();
			glActiveTexture(GL_TEXTURE1);
			waves.bind();
		}
		writeToPrev = !writeToPrev;
		renderer.render(water, waterTexShader);
		/*if (writeToPrev)
			prevFbo.unbind();
		else
			fbo.unbind();*/
		normalFbo.bind();
		glActiveTexture(GL_TEXTURE0);
		if (!writeToPrev)
			prevWaves.bind();
		else
			waves.bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.render(water, normalTexShader);
		normalFbo.unbind();
		glViewport(0, 0, camera.width, camera.height);
	}

	void KaczkaDemo::render()
	{
		renderWater();
	}
}

