#include "application.h"

namespace thunder
{
	Application* Application::s_instance = nullptr;


	Application::Application()
	{
		if (s_instance == nullptr)
			s_instance = new Application;


	}

	Application::~Application()
	{
		delete s_instance;
		s_instance = nullptr;
	}


	void Application::init()
	{
		_window = std::make_shared<window>();

	}

}

