#include "application.h"

namespace thunder
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		if (s_instance == nullptr)
			s_instance = this;

		init();
	}

	Application::~Application()
	{
		delete s_instance;
		s_instance = nullptr;
	}


	void Application::init()
	{
		_window = std::make_shared<window>(800, 600);
		_camera = std::make_shared<Camera>();

		//设置函数回调
		_window->setCallBack([ca = _camera](double x, double y){
			ca->ProcessMouseMovement(x, y);
			});
	}

	void Application::Run()
	{
		float lastFrame = 0.0f; // Time of last frame
		while (!glfwWindowShouldClose(_window->Getwindow()))
		{
			float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			processInput(deltaTime);

			_scene->OnUpdate(deltaTime);

		}

		glfwSwapBuffers(_window->Getwindow());
		glfwPollEvents();
	}

	void Application::processInput(float deltaTime)
	{
		if (glfwGetKey(_window->Getwindow(), GLFW_KEY_W) == GLFW_PRESS)
			_camera->ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(_window->Getwindow(), GLFW_KEY_S) == GLFW_PRESS)
			_camera->ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(_window->Getwindow(), GLFW_KEY_A) == GLFW_PRESS)
			_camera->ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(_window->Getwindow(), GLFW_KEY_D) == GLFW_PRESS)
			_camera->ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(_window->Getwindow(), GLFW_KEY_Q) == GLFW_PRESS)
			_camera->ProcessKeyboard(UP, deltaTime);
		if (glfwGetKey(_window->Getwindow(), GLFW_KEY_E) == GLFW_PRESS)
			_camera->ProcessKeyboard(DOWN, deltaTime);
	}

}

