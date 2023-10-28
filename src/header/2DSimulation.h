#include<glm/glm.hpp>
#include<vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class point
{
public:
	point() {}
	~point() = default;

	void setPosition(glm::vec3 pos) { _position = pos; }

private:
	glm::vec3 _position;

};


class render
{
public:
	render() { init(); }

	void init();
	unsigned int setVBO(float* vectives, unsigned int* indices);
	void SetShader();
	void run();
	static void drawQuad();


private:
	std::vector<point> PointList;

	GLFWwindow* _window;
	GLuint _vao, _vao2;
	unsigned int shaderProgram;
};

