#include"2DSimulation.h"
#include <iostream>
#include <fstream>
#include <random>

#define count 100000
static std::vector<b2Body*> list;
static std::vector<float> old_float;

Sdata s_data;

//����ߴ�
float sizeX = 0.025f;
float sizeY = 0.025f;

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
	vertices[0].Set(-1.5f, 0.0f); // ��һ������
	vertices[1].Set(-1.5f, -0.5f);  // �ڶ�������
	vertices[2].Set( 1.5f, -0.8f);  // �ڶ�������
	vertices[3].Set( 1.5f, -1.3f);  // �ڶ�������
	groundShape.Set(vertices, 4);  // ���ö�������Ͷ�����

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundShape;
	groundFixtureDef.restitution = 0.8f;  // ���õ��浯��ϵ��

	groundBody->CreateFixture(&groundFixtureDef);

	// �������İ�
	b2BodyDef leftWallBodyDef;
	leftWallBodyDef.position.Set(-2.0f, 0.0f); // ������������ (-1.0, 0.5)

	b2Body* leftWallBody = _world.CreateBody(&leftWallBodyDef);

	b2PolygonShape leftWallBox;
	leftWallBox.SetAsBox(0.2f, 5.0f); // �����Ŀ��Ϊ 0.02���߶�Ϊ 0.5

	b2FixtureDef leftWallFixtureDef;
	leftWallFixtureDef.shape = &leftWallBox;
	leftWallFixtureDef.restitution = 0.8f;  // ���õ���ϵ��

	leftWallBody->CreateFixture(&leftWallFixtureDef);

	// �����Ҳ�İ�
	b2BodyDef rightWallBodyDef;
	rightWallBodyDef.position.Set(2.1f, 0.0f); // ������������ (1.0, 0.5)

	rightWallBody = _world.CreateBody(&rightWallBodyDef);

	b2PolygonShape rightWallBox;
	rightWallBox.SetAsBox(0.1f, 5.0f); // �����Ŀ��Ϊ 0.02���߶�Ϊ 0.5

	b2FixtureDef rightWallFixtureDef;
	rightWallFixtureDef.shape = &rightWallBox;
	rightWallFixtureDef.restitution = 0.8f;  // ���õ���ϵ��

	rightWallBody->CreateFixture(&rightWallFixtureDef);
}

void scene::createInstance()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);

	double random_num1 = dis(gen);
	double random_num2 = dis(gen);
	// ��������
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(random_num1, 1.5f);
	b2Body* body = _world.CreateBody(&bodyDef);
	body->SetBullet(true);
	body->SetAngularDamping(0.8f); // ����һ������0��ֵ���Ա����ķ������ת����

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(sizeX, sizeY);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 1.0f;
	fixtureDef.restitution = 0.5f;  // �����һ�������õ���ϵ��

	body->CreateFixture(&fixtureDef);
	list.push_back(body);
}

void scene::update()
{
	_world.Step(timeStep, velocityIterations, positionIterations);
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

	for (int i = 0; i != count; i++)
	{
		s_data.instance_old_pos.push_back(glm::vec3(0, 0, 0));
		s_data.instance_new_pos.push_back(glm::vec3(0, 0, 0));
		s_data.instance_float.push_back(0.0);
		old_float.push_back(0.0);
	}

	initVAO();
	setshader();

}

void Render::initVAO()
{
	float vertices[] = {
		// ��������
		-sizeX, -sizeY, 0.0f,
		 sizeX, -sizeY, 0.0f,
		 sizeX,  sizeY, 0.0f,
		-sizeX,  sizeY, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint VBO, EBO;
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ������������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &_vbo1);
	glGenBuffers(1, &_vbo2);
	glGenBuffers(1, &_vbo3);

	// �󶨺�����һ��VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo1);
	glBufferData(GL_ARRAY_BUFFER, s_data.instance_old_pos.size() * 3 * sizeof(float), &s_data.instance_old_pos[0], GL_DYNAMIC_DRAW);

	// �󶨺����ڶ���VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo2);
	glBufferData(GL_ARRAY_BUFFER, s_data.instance_new_pos.size() * 3 * sizeof(float), &s_data.instance_new_pos[0], GL_DYNAMIC_DRAW);

	// �󶨺���������VBO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo3);
	glBufferData(GL_ARRAY_BUFFER, s_data.instance_float.size() * 1 * sizeof(float), &s_data.instance_float[0], GL_DYNAMIC_DRAW);

	/*************ʵ������1******************/
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1);

	// ���
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Render::setshader()
{
	// ������ɫ����GLSLԴ����
	const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 velocity;
layout (location = 2) in vec3 newpos;
layout (location = 3) in float rotate;

out vec3 outcolor;

void main()
{
    // ƽ�Ƶ�ԭ��
    vec4 posTranslated = vec4(aPos,1.0);

    // ��ת������X, Yƽ���ϣ�
    float cosTheta = cos(rotate);
    float sinTheta = sin(rotate);
    vec4 posRotated;
    posRotated.x = cosTheta * posTranslated.x - sinTheta * posTranslated.y;
    posRotated.y = sinTheta * posTranslated.x + cosTheta * posTranslated.y;
    posRotated.z = posTranslated.z;  // ��Z����û�б仯
    posRotated.w=1.0;

    // ƽ�Ƶ���λ��
    gl_Position = posRotated + vec4(newpos,1.0);
    outcolor=velocity;
}
)glsl";

	// Ƭ����ɫ����GLSLԴ����
	const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec3 outcolor;
void main()
{
    //FragColor = vec4(outcolor, 1.0);
    FragColor = vec4(0.0,1.0,0.0, 1.0);
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

void Render::update()
{
	// ���µ�һ��VBO (_vbo1)
	glBindBuffer(GL_ARRAY_BUFFER, _vbo1);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(float) * 3, &s_data.instance_old_pos[0]);

	// ���µڶ���VBO (_vbo2)
	glBindBuffer(GL_ARRAY_BUFFER, _vbo2);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(float) * 3, &s_data.instance_new_pos[0]);

	// ���µ�����VBO (_vbo3)
	glBindBuffer(GL_ARRAY_BUFFER, _vbo3);
	glBufferSubData(GL_ARRAY_BUFFER, 0, num * sizeof(float), &s_data.instance_float[0]);

	// ���
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::updateData()
{
	_scene->update();
	for (int i = 0; i != num; i++)
	{
		b2Vec2 position = list[i]->GetPosition();
		float rotate = list[i]->GetAngle() - old_float[i];
		b2Vec2 speed = list[i]->GetLinearVelocity();

		s_data.instance_old_pos[i].x = speed.x;
		s_data.instance_old_pos[i].y = speed.y;

		s_data.instance_new_pos[i].x = position.x;
		s_data.instance_new_pos[i].y = position.y;
		s_data.instance_float[i] = rotate;
		old_float[i] = list[i]->GetAngle();
	}
	update();
}

void Render::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, 800, 600);
	int a = 0;

	while (!glfwWindowShouldClose(_window))
	{
		if (a % 1 == 0)
			createInstance();
		a++;

		updateData();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(_vao);
		glDrawElementsInstanced(
			GL_TRIANGLES,      // ����ģʽ
			6 * num,       // ��������
			GL_UNSIGNED_INT,   // ��������
			0,                 // ���������ƫ����
			num     // ʵ������
		);

		glfwSwapBuffers(_window);
		glfwPollEvents();
		glfwPollEvents();
	}
}

