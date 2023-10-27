//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <Box2D/Box2D.h>
//#include <iostream>
//
//
//int main()
//{
//	if (!glfwInit()) {
//		std::cout << "GLFW initialization failed" << std::endl;
//		return -1;
//	}
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
//
//	GLFWwindow* window = glfwCreateWindow(800, 600, "Box2D with GLFW and GLAD", NULL, NULL);
//	if (!window) {
//		std::cout << "Window creation failed" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//		std::cout << "GLAD initialization failed" << std::endl;
//		return -1;
//	}
//
//	// �������������
//	b2Vec2 gravity(0.0f, -9.8f);
//
//	// ����һ��Box2D����
//	b2World world(gravity);
//
//	// ��������
//	{
//		b2BodyDef groundBodyDef;
//		groundBodyDef.position.Set(0.0f, -10.0f);
//
//		b2Body* groundBody = world.CreateBody(&groundBodyDef);
//
//		b2PolygonShape groundBox;
//		groundBox.SetAsBox(50.0f, 10.0f);
//
//		groundBody->CreateFixture(&groundBox, 0.0f);
//	}
//
//	// ����һ������ķ���
//	{
//		b2BodyDef bodyDef;
//		bodyDef.type = b2_dynamicBody;
//		bodyDef.position.Set(0.0f, 4.0f);
//
//		b2Body* body = world.CreateBody(&bodyDef);
//
//		b2PolygonShape dynamicBox;
//		dynamicBox.SetAsBox(1.0f, 1.0f);
//
//		b2FixtureDef fixtureDef;
//		fixtureDef.shape = &dynamicBox;
//		fixtureDef.density = 1.0f;
//		fixtureDef.friction = 0.3f;
//
//		body->CreateFixture(&fixtureDef);
//	}
//
//	// ģ����������
//	float timeStep = 1.0f / 60.0f;
//	int32 velocityIterations = 6;
//	int32 positionIterations = 2;
//
//	for (int32 i = 0; i < 60; ++i) {
//		world.Step(timeStep, velocityIterations, positionIterations);
//
//		b2Body* body = world.GetBodyList();
//		b2Vec2 position = body->GetPosition();
//		float angle = body->GetAngle();
//
//		std::cout << "Iteration: " << i
//			<< " | x: " << position.x
//			<< " | y: " << position.y
//			<< " | angle: " << angle << std::endl;
//	}
//
//	while (!glfwWindowShouldClose(window)) {
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		// ģ����������
//		world.Step(timeStep, velocityIterations, positionIterations);
//
//		b2Body* body = world.GetBodyList();
//		b2Vec2 position = body->GetPosition();
//		// ...���˴�����ʹ��OpenGL�����Ʒ��飩
//
//		// ��OpenGL���Ʒ��飨��ʾ����
//		glBegin(GL_QUADS);
//		glVertex2f(position.x - 1.0f, position.y - 1.0f);
//		glVertex2f(position.x + 1.0f, position.y - 1.0f);
//		glVertex2f(position.x + 1.0f, position.y + 1.0f);
//		glVertex2f(position.x - 1.0f, position.y + 1.0f);
//		glEnd();
//
//		// ��������������ѯ�¼�
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glfwDestroyWindow(window);
//	glfwTerminate();
//
//}
