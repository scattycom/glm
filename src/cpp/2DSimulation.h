#include"TestFBO.h"
#include <Box2D/Box2D.h>
#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include "camera.h"

struct Sdata
{
	std::vector<glm::vec3> instance_old_pos;
	std::vector<glm::vec3> instance_new_pos;
	std::vector<float> instance_float;
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
	~Render();
	void init();
	void initVAO();
	void setshader();
	void run();
private:
	void createInstance()
	{
		num++;
		_scene->createInstance();
	}
	void update();
	void updateData();

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	GLFWwindow* _window;
	GLuint _vao, _vbo1, _vbo2, _vbo3;
	std::unique_ptr<scene> _scene;
	unsigned int shaderProgram;
	Camera* _camera;

	int num = 0;
};
