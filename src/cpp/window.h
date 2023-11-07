#include<string>
#include "GLFW/glfw3.h"
#include<functional>

namespace thunder
{
	class window
	{
	public:
		using funCallBack = std::function<void(double, double)>;

		window(int width = 800,int height = 600,const std::string& name="Thunder");

		void init();
		GLFWwindow* Getwindow() { return _window; }
		int Getwidth() { return _width; }
		int Getheight() { return _height; }

		void setSize(int width, int height) { _width = width; _height = height; }

		void setCallBack(funCallBack callback) { _callback = callback; }
		funCallBack GetCallBack() { return _callback; }
	private:
		int _width, _height;
		const std::string _name;
		GLFWwindow* _window;
		funCallBack _callback;
	};
}

