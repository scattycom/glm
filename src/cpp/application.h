#include<memory>
#include"window.h"

namespace thunder
{
	class Application
	{
	public:
		Application();
		~Application();

		void init();
		static Application* GetApplication() { return s_instance; }

	private:


	private:
		static Application* s_instance;
		std::shared_ptr<window> _window;
	};


}
