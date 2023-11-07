#pragma once
#include<memory>
#include"window.h"
#include"camera.h"
#include"scene.h"

namespace thunder
{
	class Application
	{
	public:
		Application();
		~Application();

		void init();

		static Application* GetAppInstance() { return s_instance; }
		std::shared_ptr<window> GetWindow() { return _window; }
		void Run();
	private:

		void processInput(float deltaTime);

	private:
		static Application* s_instance;
		std::shared_ptr<window> _window;
		std::shared_ptr<Camera> _camera;

		std::shared_ptr<layer> _scene;
	};

}
