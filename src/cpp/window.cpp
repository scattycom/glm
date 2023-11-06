#include "window.h"


namespace thunder
{

	window::window(int width, int height, const std::string& name)
		:_width(width),_height(height),_name(name)
	{
		init();
	}

	void window::init()
	{
		if (!glfwInit())
			return;

		_window = glfwCreateWindow(_width, _height, _name.c_str(), NULL, NULL);
		if (!_window)
		{
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(_window);
	}




}

