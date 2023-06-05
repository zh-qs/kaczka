#pragma once

#include <glm/glm.hpp>
#include <random>

namespace kaczka
{
	class RandomPath
	{
		glm::vec3 deBoorPoints[4];
		std::uniform_real_distribution<float> random;
		std::random_device dev;
		float t = 0.0f;

		glm::vec3 randomVec();
		glm::vec3 getPointOnCurve(glm::vec3& derivative);
	public:
		RandomPath();
		glm::vec3 next(float dt, glm::vec3& derivative);
	};
}