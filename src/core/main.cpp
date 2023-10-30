#include"src/header/TestFBO.h"
#include"src/header/2DSimulation.h"
#include <Box2D/Box2D.h>
#include <iostream>
#if !ACTIVE
int main()
{
	/*Render* test = new Render;
	test->run();*/
	// ����Box2D����
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	// ��������
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 0.5f);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundBox;
	groundFixtureDef.restitution = 0.8f;  // ���õ��浯��ϵ��

	groundBody->CreateFixture(&groundFixtureDef);

	// ��������
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 1.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.25f, 0.25f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;  // �����һ�������õ���ϵ��

	body->CreateFixture(&fixtureDef);

	// ģ��
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 5;

	for (int32 i = 0; i < 360; ++i) {
		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float angle = body->GetAngle();

		std::cout << "iteration: " << i << " x: " << position.x << " y: " << position.y << " angle: " << angle << std::endl;
	}

	return 0;



}
#else
#endif

int main()
{
	Render* re = new Render;
	re->run();
}
