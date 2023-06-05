#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace kaczka
{
	class Camera {
		float rotx = 0, roty = 0.5f;
		float distance = 1.0f;
		glm::vec3 target = { 0.0f, 0.0f, 0.0f };

		static constexpr float fov = glm::quarter_pi<float>(),
			near = 0.1f,
			far = 100.0f;

	public:
		int width, height;

		void zoom(float scale);
		void move(const glm::vec3& v);
		void rotate(float angx, float angy);

		glm::vec3 getPosition() const;
		glm::mat4 getViewMatrix() const;
		glm::mat4 getProjMatrix() const;
	};
}