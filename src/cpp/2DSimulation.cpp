#include"../header/2DSimulation.h"
#include <iostream>
#include <fstream>

void writeNameToFile(const std::string& name) {
	// ����һ������ļ������󲢴��ļ�
	std::ofstream outfile("output.txt", std::ios::app);  // ��ģʽ std::ios::app ��ʾ���ļ�ĩβ׷��
	// �����ϣ��׷�ӣ�ֻ��ʹ�� std::ofstream outfile("output.txt");

// ����ļ��Ƿ�ɹ���
	if (outfile.is_open()) {
		// �� name д���ļ�
		outfile << name << std::endl;

		// �ر��ļ�
		outfile.close();
	}
	else {
		// �ļ���ʧ�ܣ���ӡ������Ϣ
		std::cerr << "Unable to open file for writing." << std::endl;
	}
}

scene::scene() : _world(b2Vec2(0.0f, -9.8f)) // ��ʼ���б�
{
	// ��������
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -0.5f);

	b2Body* groundBody = _world.CreateBody(&groundBodyDef);

	//b2PolygonShape groundBox;
	//groundBox.SetAsBox(1.0f, 0.0f);

	b2PolygonShape groundShape;

	// �����������㣬����һ����б�ĵ���
	b2Vec2 vertices[4];
	vertices[0].Set(-1.0f, 0.0f); // ��һ������
	vertices[1].Set(-1.0f, -0.1f);  // �ڶ�������
	vertices[2].Set(2.0f, -0.8f);  // �ڶ�������
	vertices[3].Set(2.0f, -1.0f);  // �ڶ�������
	groundShape.Set(vertices, 4);  // ���ö�������Ͷ�����

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.restitution = 0.8f;  // ���õ��浯��ϵ��

	groundBody->CreateFixture(&groundFixtureDef);

	// �������İ�
	b2BodyDef leftWallBodyDef;
	leftWallBodyDef.position.Set(-0.5f, 0.0f); // ������������ (-1.0, 0.5)

	b2Body* leftWallBody = _world.CreateBody(&leftWallBodyDef);

	b2PolygonShape leftWallBox;
	leftWallBox.SetAsBox(0.0f, 2.0f); // �����Ŀ��Ϊ 0.02���߶�Ϊ 0.5

	b2FixtureDef leftWallFixtureDef;
	leftWallFixtureDef.shape = &leftWallBox;
	leftWallFixtureDef.restitution = 0.8f;  // ���õ���ϵ��

	leftWallBody->CreateFixture(&leftWallFixtureDef);

	// �����Ҳ�İ�
	b2BodyDef rightWallBodyDef;
	rightWallBodyDef.position.Set(0.3f, 0.0f); // ������������ (1.0, 0.5)

	rightWallBody = _world.CreateBody(&rightWallBodyDef);

	b2PolygonShape rightWallBox;
	rightWallBox.SetAsBox(0.3f, 5.0f); // �����Ŀ��Ϊ 0.02���߶�Ϊ 0.5

	b2FixtureDef rightWallFixtureDef;
	rightWallFixtureDef.shape = &rightWallBox;
	rightWallFixtureDef.restitution = 0.8f;  // ���õ���ϵ��

	rightWallBody->CreateFixture(&rightWallFixtureDef);


}

void scene::createInstance()
{
	// ��������
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
	fixtureDef.restitution = 0.5f;  // �����һ�������õ���ϵ��
	body->CreateFixture(&fixtureDef);
	list.push_back(body);
}

void scene::update()
{
	_world.Step(timeStep, velocityIterations, positionIterations);
}

void initVerAndIndex(std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	for (int i = 0; i < 1000; i++)
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

	for (int i = 0; i != 0; i++)
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

// ��ʼ���׶Σ���ִ��һ��
void Render::initVAO(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// ���½׶Σ�ÿ��vertices����ʱִ��
void Render::updateVAO(std::vector<float> vertices, std::vector<unsigned int> indices)
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*num * sizeof(float), vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*num * sizeof(unsigned int), indices.data());
}

void Render::SetShader()
{
	// ������ɫ����GLSLԴ����
	const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)glsl";

	// Ƭ����ɫ����GLSLԴ����
	const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)glsl";

	// ����������ɫ��
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// ��鶥����ɫ�������Ƿ�ɹ�
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ����Ƭ����ɫ��
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// ���Ƭ����ɫ�������Ƿ�ɹ�
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// ������ɫ������������ɫ��
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// ��������Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// ɾ����ɫ�����������Ѿ����ӵ���ɫ�����򣬲�����Ҫ
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

		// ʹ�ü򵥵����Һ����ð������ƶ�
		float xOffset =0.5f*sin(0.1f * timeElapsed);
		b2Vec2 newPosition(xOffset+0.5, 0.0f); // ����λ��
		_scene->rightWallBody->SetTransform(newPosition, 0); // �ڶ�����������ת�Ƕȣ�������Ϊ0


		_scene->update();
		updatePosition();

		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6 * num, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

}



