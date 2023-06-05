#include "random_path.h"

glm::vec3 kaczka::RandomPath::randomVec()
{
	return { random(dev),0.0f,random(dev) };
}

glm::vec3 kaczka::RandomPath::getPointOnCurve(glm::vec3& derivative)
{
	// de Boor algorithm
	float n01 = 1;
	float n10 = n01 * (1 - t), n11 = n01 * t;
	float n2m1 = n10 * (1 - t) / 2.0f, n20 = n10 * (t + 1) / 2.0f + n11 * (2 - t) / 2.0f, n21 = n11 * t / 2.0f;
	float n3m2 = n2m1 * (1 - t) / 3.0f, n3m1 = n2m1 * (t + 2) / 3.0f + n20 * (2 - t) / 3.0f, n30 = n20 * (t + 1) / 3.0f + n21 * (3 - t) / 3.0f, n31 = n21 * t / 3.0f;
	
	derivative = (deBoorPoints[1] - deBoorPoints[0]) * n2m1 + (deBoorPoints[2] - deBoorPoints[1]) * n20 + (deBoorPoints[3] - deBoorPoints[2]) * n21;
	return deBoorPoints[0] * n3m2 + deBoorPoints[1] * n3m1 + deBoorPoints[2] * n30 + deBoorPoints[3] * n31;
}

kaczka::RandomPath::RandomPath()
{
	for (int i = 0; i < 4; ++i)
		deBoorPoints[i] = randomVec();
}

glm::vec3 kaczka::RandomPath::next(float dt, glm::vec3& derivative)
{
	auto result = getPointOnCurve(derivative);
	t += dt;
	if (t > 1.0f)
	{
		for (int i = 0; i < 3; ++i)
			deBoorPoints[i] = deBoorPoints[i + 1];
		deBoorPoints[3] = randomVec();
		t -= 1.0f;
	}
	return result;
}
