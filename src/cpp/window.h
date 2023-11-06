#include<string>
#include "GLFW/glfw3.h"

namespace thunder
{
	class window
	{
	public:
		window(int width = 800,int height = 600,const std::string& name="Thunder");

		void init();
		GLFWwindow* Getwindow() { return _window; }
		int Getwidth() { return _width; }
		int Getheight() { return _height; }

	private:
		int _width, _height;
		const std::string _name;
		GLFWwindow* _window;

	};
}

