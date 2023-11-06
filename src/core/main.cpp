#include"src/cpp/TestFBO.h"
#include"src/cpp/2DSimulation.h"
#include <Box2D/Box2D.h>
#include <iostream>


#if 0
int main()
{
	/*Render* test = new Render;
	test->run();*/
	// 创建Box2D世界
	b2Vec2 gravity(0.0f, -10.0f);
	b2World world(gravity);

	// 创建地面
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 0.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 0.5f);

	b2FixtureDef groundFixtureDef;
	groundFixtureDef.shape = &groundBox;
	groundFixtureDef.restitution = 0.8f;  // 设置地面弹性系数

	groundBody->CreateFixture(&groundFixtureDef);

	// 创建方块
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 1.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(0.25f, 0.25f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f;  // 添加这一行以设置弹性系数

	body->CreateFixture(&fixtureDef);

	// 模拟
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 10;
	int32 positionIterations = 5;

	for (int32 i = 0; i < 360; ++i) {
		world.Step(timeStep, velocityIterations, positionIterations);
		b2Vec2 position = body->GetPosition();
		float angle = body->GetAngle();

		std::cout << "iteration: " << i << " x: " << position.x << " y: " << position.y << " angle: " << angle << std::endl;
	}

	return 0;



}
#else
#endif

//int main()
//{
//	Render* re = new Render;
//	re->run();
//}

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct Vertex {
	glm::vec3 Position; // 位置
	glm::vec3 Normal;   // 法线
	// 你还可以添加其他顶点属性，比如纹理坐标、切线等
};

std::vector<Vertex> vertices; // 存储所有顶点
std::vector<unsigned int> indices; // 存储所有索引


void ProcessNode(aiNode* node, const aiScene* scene) {
	// 处理当前节点的所有网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		// 在这里处理网格
		std::cout << "Mesh name: " << mesh->mName.C_Str() << std::endl;
	}

	// 递归处理每个子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

#if 0
int main() {
	Assimp::Importer importer;

	// 在这里替换为你的模型文件路径
	const std::string modelPath = "../../Room#1.obj";

	// 读取模型文件
	const aiScene* scene = importer.ReadFile(modelPath,
		aiProcess_Triangulate |       // 将所有模型的所有面都转换为三角形
		aiProcess_FlipUVs |           // 翻转y轴上的纹理坐标
		aiProcess_CalcTangentSpace |  // 计算切线和副切线
		aiProcess_GenNormals);        // 创建法线

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return -1;
	}

	// 处理Assimp的根节点
	ProcessNode(scene->mRootNode, scene);

	return 0;
}
#endif

#if 1
int main()
{
	Render* demo = new Render;
	demo->run();

}

#endif

