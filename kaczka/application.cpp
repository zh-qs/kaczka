#include "application.h"
#include "exception.h"
#include <chrono>
#include <thread>

namespace kaczka
{
	static void glfw_error_callback(int error, const char* description)
	{
		fprintf(stderr, "GLFW Error %d: %s\n", error, description);
	}

	static glm::vec2 prevMousePos = { 0.0f,0.0f };
	static glm::vec2 mousePos = { 0.0f,0.0f };
	static bool rButtonPressed = false, lButtonPressed = false;

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			rButtonPressed = action == GLFW_PRESS;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			lButtonPressed = action == GLFW_PRESS;
		}
	}

	//static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
	//{
	//	prevMousePos = mousePos;
	//	mousePos.x = xpos;
	//	mousePos.y = ypos;
	//	auto d = prevMousePos - mousePos;
	//	printf("(%f,%f)\n", d.x, d.y);
	//}

	Application::Application(int width, int height, const char* title, const glm::vec4& backColor) : backColor(backColor), demo(camera)
	{
		camera.width = width;
		camera.height = height;

		glfwSetErrorCallback(glfw_error_callback);
		if (!glfwInit())
			THROW_EXCEPTION;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		if (window == nullptr)
			THROW_EXCEPTION;
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			THROW_EXCEPTION;

		glfwSetMouseButtonCallback(window, mouseButtonCallback);

		demo.init();
	}

	void Application::run()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();
			glfwGetFramebufferSize(window, &camera.width, &camera.height);
			glViewport(0, 0, camera.width, camera.height);
			glClearColor(backColor.x * backColor.w, backColor.y * backColor.w, backColor.z * backColor.w, backColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);
			prevMousePos = mousePos;
			mousePos.x = xpos;
			mousePos.y = ypos;

			if (rButtonPressed)
			{
				glm::vec2 mouseDelta = 0.01f * (mousePos - prevMousePos);
				camera.rotate(mouseDelta.x, mouseDelta.y);
			}
			if (lButtonPressed)
			{
				glm::vec2 mouseDelta = 0.01f * (mousePos - prevMousePos);
				camera.zoom(mouseDelta.y);
			}

			auto start = std::chrono::high_resolution_clock::now();

			demo.update(FRAME_TIME);
			demo.render();

			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end - start;
			if (FRAME_TIME - diff.count() > 0)
				std::this_thread::sleep_for(std::chrono::duration<double>(FRAME_TIME - diff.count()));

			glfwSwapBuffers(window);
		}
	}
}
