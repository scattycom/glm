#include"TestFBO.h"
#include <Box2D/Box2D.h>
#include <memory>
#include <glm/glm.hpp>
#include<vector>

static std::vector<float> vertices;
static std::vector<unsigned int> indices;
static std::vector<b2Body*> list;

class scene
{
public:
	scene();

	void createInstance();
	void update();
private:
	b2World _world;

	float timeStep = 1.0f / 60.0f;  // 模拟的时间步长
	int32 velocityIterations = 6; // 速度迭代次数
	int32 positionIterations = 2; // 位置迭代次数
};

class Render
{
public:
	Render();

	void init();
	void createInstance();
	void initVAO(std::vector<float> vertices, std::vector<unsigned int> indices);
	void updateVAO(std::vector<float> vertices, std::vector<unsigned int> indices);
	
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
