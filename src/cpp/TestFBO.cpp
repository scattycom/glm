#include "TestFBO.h"
#include<iostream>

void TestFBO::init()
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
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
	float vertices[] =
	{
		-0.5,-0.5,0,0,0,
		 0.5,-0.5,0,1,0,
		 0.5, 0.5,0,1,1,
		-0.5, 0.5,0,0,1
	};

	float vertices1[] =
	{
		0,0,0,0,0,
		1,0,0,1,0,
		1,1,0,1,1,
		0,1,0,0,1
	};

	unsigned int indices[] =
	{
		0,1,2,
		0,2,3
	};

	_vao = setVBO(vertices, indices);
	_vao2 = setVBO(vertices1, indices);

	SetShader();
	SetShader2();
	SetFBO();
}

unsigned int TestFBO::setVBO(float* vertices, unsigned int* indices)
{
	unsigned int vao,vbo, ibo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 20*sizeof(float), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return vao;
}

void TestFBO::SetShader()
{
	// 顶点着色器的GLSL源代码
	const char *vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
	texCoord=aTexCoord;
}
)glsl";

	// 片段着色器的GLSL源代码
	const char *fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D _sampler;

void main()
{
    //FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    vec4 sampledColor = texture(_sampler, texCoord);
    FragColor=sampledColor;
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

void TestFBO::SetShader2()
{
	// 顶点着色器的GLSL源代码
	const char *vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
	texCoord=aTexCoord;
}
)glsl";

	// 片段着色器的GLSL源代码
	const char *fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D _sampler;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    //vec4 sampledColor = texture(_sampler, texCoord);
    //FragColor=sampledColor;
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
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader);
	glAttachShader(shaderProgram2, fragmentShader);
	glLinkProgram(shaderProgram2);

	// 检查链接是否成功
	glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器对象，它们已经链接到着色器程序，不再需要
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void TestFBO::SetFBO()
{
	// 1. 创建FBO
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// 2. 创建纹理
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glGenerateMipmap(GL_TEXTURE_2D);  // 生成多级渐远纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // 设置过滤模式以使用mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 3. 将纹理附加到FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// 4. （可选）创建和附加深度缓冲
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// 检查FBO是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TestFBO::run()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glActiveTexture(GL_TEXTURE0);  // 激活纹理单元0
	glBindTexture(GL_TEXTURE_2D, texture);  // 绑定纹理
	glUniform1i(glGetUniformLocation(shaderProgram, "_sampler"), 0);
	while (!glfwWindowShouldClose(_window))
	{
		// 渲染到FBO
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glViewport(0, 0, 800, 600);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram2);
		glBindVertexArray(_vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		// 解绑FBO并渲染到屏幕
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (1)
		{
			glGenerateMipmap(GL_TEXTURE_2D);  // 生成多级渐远纹理
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // 设置过滤模式以使用mipmap
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glBindVertexArray(_vao2);
		glUseProgram(shaderProgram);
		glViewport(0, 0, 800, 600);  // 注意这里可能需要设置回默认窗口尺寸
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}
