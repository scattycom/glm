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
		glfwSetWindowUserPointer(_window, this);

		glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* Window, int width, int height) {
			window* ptr = static_cast<window*>(glfwGetWindowUserPointer(Window));
			if (ptr != nullptr)
			{
				ptr->setSize(width, height);
			}
		});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* Window,double xpos,double ypos) {
			window* ptr = static_cast<window*>(glfwGetWindowUserPointer(Window));
			if (ptr != nullptr)
			{
				ptr->GetCallBack()(xpos, ypos);
			}
		});

	}




}

