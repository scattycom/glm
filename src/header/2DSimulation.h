#include"TestFBO.h"
#include <Box2D/Box2D.h>
#include <memory>
#include <glm/glm.hpp>
#include<vector>


struct point
{
	glm::vec3 p0;
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
};

class scene
{
public:
	scene();

	void createInstance();
	void update();
	b2Body* rightWallBody;
private:
	b2World _world;

	float timeStep = 1.0f / 60.0f;  // 模拟的时间步长
	int32 velocityIterations = 10; // 速度迭代次数
	int32 positionIterations = 10; // 位置迭代次数
};

class Render
{
public:
	Render();

	void init();
	void createInstance();

	void initVAO();
	void updateVBO();

	void SetShader();
	void run();
	void updatePosition();
private:
	void caculate(int i, glm::vec2 pos);

	GLFWwindow* _window;
	GLuint _vao, _vbo, _ibo;
	std::unique_ptr<scene> _scene;
	unsigned int shaderProgram;

	int num = 0;
};
