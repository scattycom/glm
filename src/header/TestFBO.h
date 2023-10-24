#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>

class TestFBO
{
public:
	TestFBO() { init(); }
	~TestFBO() {}
	void init();

	void SetShader();
	void SetShader2();
	void SetFBO();
	unsigned int setVBO(float* vectives, unsigned int* indices);
	void run();

private:
	GLFWwindow* _window;
	GLuint _vao, _vao2;
	unsigned int shaderProgram, shaderProgram2, FBO, texture;


};
