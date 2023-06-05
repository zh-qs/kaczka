#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "kaczka_demo.h"

namespace kaczka
{
	class Application {
		GLFWwindow* window;
		Camera camera;
		KaczkaDemo demo;
		glm::vec4 backColor;

		static constexpr double FPS = 80.0f;
		static constexpr double FRAME_TIME = 1.0f / FPS;

	public:
		Application(int width, int height, const char* title, const glm::vec4& backColor);
		void run();
	};
}