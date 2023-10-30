#include"../header/2DSimulation.h"
#include <iostream>
#include <fstream>

void writeNameToFile(const std::string& name) {
	// 创建一个输出文件流对象并打开文件
	std::ofstream outfile("output.txt", std::ios::app);  // 打开模式 std::ios::app 表示在文件末尾追加
	// 如果不希望追加，只需使用 std::ofstream outfile("output.txt");

// 检查文件是否成功打开
	if (outfile.is_open()) {
		// 将 name 写入文件
		outfile << name << std::endl;

		// 关闭文件
		outfile.close();
	}
	else {
		// 文件打开失败，打印错误信息
		std::cerr << "Unable to open file for writing." << std::endl;
	}
}

scene::scene() : _world(b2Vec2(0.0f, -9.8f)) // 初始化列表
{
	// 创建地面
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -1.0f);

	b2Body* groundBody = _world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 0.5f);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundBox;
	groundFixtureDef.restitution = 0.8f;  // 设置地面弹性系数

	groundBody->CreateFixture(&groundFixtureDef);
}

void scene::createInstance()
{
	// 创建方块
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.9f);
	b2Body* body = _world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.05f, 0.05f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;  // 添加这一行以设置弹性系数

	body->CreateFixture(&fixtureDef);
	list.push_back(body);
}

void scene::update()
{
	_world.Step(timeStep, velocityIterations, positionIterations);
}

void initVerAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	for (int i = 0; i < 100; i++)
	{
		vertices.push_back(-0.025);
		vertices.push_back(-0.025);
		vertices.push_back(-0.5);

		vertices.push_back(0.025);
		vertices.push_back(-0.025);
		vertices.push_back(-0.5);

		vertices.push_back(0.025);
		vertices.push_back(0.025);
		vertices.push_back(-0.5);

		vertices.push_back(-0.025);
		vertices.push_back(0.025);
		vertices.push_back(-0.5);

		indices.push_back(0 + 4 * i);
		indices.push_back(1 + 4 * i);
		indices.push_back(2 + 4 * i);
						 
		indices.push_back(0 + 4 * i);
		indices.push_back(2 + 4 * i);
		indices.push_back(3 + 4 * i);
	}
}

Render::Render()
{
	_scene = std::make_unique<scene>();
	init();
}

void Render::init()
{
	if (!glfwInit())
		return;

	_window = glfwCreateWindow(800, 600, "FBO", NULL, NULL);
	if (!_window)
	{
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		return;
	}
	initVerAndIndex(vertices, indices);

	for (int i = 0; i != 100; i++)
	{
		createInstance();
	}
	initVAO(vertices, indices);
	SetShader();
}

void Render::createInstance()
{
	num++;

	_scene->createInstance(); 
}

// 初始化阶段：仅执行一次
void Render::initVAO(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 12* num * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6* num * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// 更新阶段：每次vertices更新时执行
void Render::updateVAO(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*num * sizeof(float), vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*num * sizeof(unsigned int), indices.data());
}

void Render::SetShader()
{
	// 顶点着色器的GLSL源代码
	const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)glsl";

	// 片段着色器的GLSL源代码
	const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)glsl";

	// 创建顶点着色器
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// 检查顶点着色器编译是否成功
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// 创建片段着色器
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// 检查片段着色器编译是否成功
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// 创建着色器程序并链接着色器
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// 检查链接是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器对象，它们已经链接到着色器程序，不再需要
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Render::updatePosition()
{
	for (int i = 0; i != num; i++)
	{
		b2Vec2 position = list[i]->GetPosition();
		caculate(i, glm::vec2{ position.x, position.y });
	}
	updateVAO(vertices, indices);
}

void Render::caculate(int i, glm::vec2 pos)
{
	float oldx, oldy;
	i *= 12;
	oldx = vertices[i] + vertices[i + 3];
	oldx /= 2;

	oldy = vertices[i + 1] + vertices[i + 10];
	oldy /= 2;
	glm::vec2 temp = { pos.x - oldx, pos.y - oldy };

	vertices[i] += temp.x;
	vertices[i + 1] += temp.y;

	vertices[i + 3] += temp.x;
	vertices[i + 4] += temp.y;

	vertices[i + 6] += temp.x;
	vertices[i + 7] += temp.y;

	vertices[i + 9] += temp.x;
	vertices[i + 10] += temp.y;
	int a = 10;
}

void Render::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, 800, 600);
	glUseProgram(shaderProgram);
	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_scene->update();
		updatePosition();

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6 * num, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

}



