#include"../header/2DSimulation.h"
#include <iostream>
#include <fstream>

static std::vector<point> vertices1;
static std::vector<unsigned int> indices;
static std::vector<b2Body*> list;

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
	groundBodyDef.position.Set(0.0f, -0.5f);

	b2Body* groundBody = _world.CreateBody(&groundBodyDef);
	
	//b2PolygonShape groundBox;
	//groundBox.SetAsBox(1.0f, 0.0f);

	b2PolygonShape groundShape;

	// 设置两个顶点，创建一个倾斜的地面
	b2Vec2 vertices[4];
	vertices[0].Set(-1.0f, 0.0f); // 第一个顶点
	vertices[1].Set(-1.0f, -0.1f);  // 第二个顶点
	vertices[2].Set(2.0f, -0.8f);  // 第二个顶点
	vertices[3].Set(2.0f, -1.0f);  // 第二个顶点
	groundShape.Set(vertices, 4);  // 设置顶点数组和顶点数

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.restitution = 0.8f;  // 设置地面弹性系数

	groundBody->CreateFixture(&groundFixtureDef);

	// 创建左侧的板
	b2BodyDef leftWallBodyDef;
	leftWallBodyDef.position.Set(-0.5f, 0.0f); // 假设板的中心在 (-1.0, 0.5)

	b2Body* leftWallBody = _world.CreateBody(&leftWallBodyDef);

	b2PolygonShape leftWallBox;
	leftWallBox.SetAsBox(0.0f, 2.0f); // 假设板的宽度为 0.02，高度为 0.5

	b2FixtureDef leftWallFixtureDef;
	leftWallFixtureDef.shape = &leftWallBox;
	leftWallFixtureDef.restitution = 0.8f;  // 设置弹性系数

	leftWallBody->CreateFixture(&leftWallFixtureDef);

	// 创建右侧的板
	b2BodyDef rightWallBodyDef;
	rightWallBodyDef.position.Set(0.3f, 0.0f); // 假设板的中心在 (1.0, 0.5)

	rightWallBody = _world.CreateBody(&rightWallBodyDef);

	b2PolygonShape rightWallBox;
	rightWallBox.SetAsBox(0.3f, 5.0f); // 假设板的宽度为 0.02，高度为 0.5

	b2FixtureDef rightWallFixtureDef;
	rightWallFixtureDef.shape = &rightWallBox;
	rightWallFixtureDef.restitution = 0.8f;  // 设置弹性系数

	rightWallBody->CreateFixture(&rightWallFixtureDef);


}

void scene::createInstance()
{
	// 创建方块
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.9f);
	b2Body* body = _world.CreateBody(&bodyDef);
	body->SetBullet(true);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.025f, 0.025f);

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

void initVerAndIndex()
{
	for (int i = 0; i < 1000; i++)
	{
		indices.push_back(0 + 4 * i);
		indices.push_back(1 + 4 * i);
		indices.push_back(2 + 4 * i);
		indices.push_back(0 + 4 * i);
		indices.push_back(2 + 4 * i);
		indices.push_back(3 + 4 * i);

		point p1
		{ 
			glm::vec3{-0.025,-0.025,-0.5},
			glm::vec3{ 0.025,-0.025,-0.5}, 
			glm::vec3{ 0.025, 0.025,-0.5}, 
			glm::vec3{-0.025, 0.025,-0.5},
		};
		vertices1.push_back(p1);
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
	initVerAndIndex();
	initVAO();
	SetShader();
}

void Render::createInstance()
{
	num++;

	_scene->createInstance(); 
}

// 初始化阶段：仅执行一次
void Render::initVAO()
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices1.size()* 12 * sizeof(float), vertices1.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Render::updateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * num * sizeof(float), vertices1.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * num * sizeof(unsigned int), indices.data());
}

// 更新阶段：每次vertices更新时执行

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
	updateVBO();
}

void Render::caculate(int a, glm::vec2 pos)
{
	float oldx, oldy;
	oldx = (vertices1[a].p0.x + vertices1[a].p1.x) / 2;
	oldy = (vertices1[a].p0.y + vertices1[a].p3.y) / 2;
	glm::vec2 temp = { pos.x - oldx, pos.y - oldy };

	vertices1[a].p0 += glm::vec3{ temp.x,temp.y,0 };
	vertices1[a].p1 += glm::vec3{ temp.x,temp.y,0 };
	vertices1[a].p2 += glm::vec3{ temp.x,temp.y,0 };
	vertices1[a].p3 += glm::vec3{ temp.x,temp.y,0 };
}

void Render::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, 800, 600);
	glUseProgram(shaderProgram);
	float timeElapsed = 0;
	int a = 0;
	while (!glfwWindowShouldClose(_window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (a%50==0&&num<20)
		{
			createInstance();
		}
		a++;

		timeElapsed += 0.2;

		// 使用简单的正弦函数让板左右移动
		float xOffset =0.5f*sin(0.1f * timeElapsed);
		b2Vec2 newPosition(xOffset+0.5, 0.0f); // 更新位置
		_scene->rightWallBody->SetTransform(newPosition, 0); // 第二个参数是旋转角度，这里设为0

		_scene->update();
		updatePosition();

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6 * num, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

}



