#pragma once
#include"src/cpp/TestFBO.h"
#include"src/cpp/2DSimulation.h"
#include"src/cpp/loader.h"
#include"src/cpp/application.h"
#include"src/vulkanAPI/context.h"

#include <Box2D/Box2D.h>
#include <iostream>

#if 0
int main()
{
	//Render* _data = new Render;
	//_data->run();
	
	//thunder::Application* app = new thunder::Application();
	//app->Run();

	thunderdemo::context::init();
	thunderdemo::context::GetInstance()->create();
	thunderdemo::context::quit();


	system("pause");

	return 0;
}
#endif
