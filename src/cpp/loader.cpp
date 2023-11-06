#include"loader.h"
#include<iostream>
#include<chrono>


struct _data {
	std::vector<std::vector<glm::vec3>> model_List;
	std::vector<std::vector<unsigned int>> model_indices;

};

static _data s_data;
static int numofmesh = 0;

void ProcessNode(aiNode* node, const aiScene* scene) {
	// 处理当前节点的所有网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		// 在这里处理网格
		std::cout << "Mesh name: " << mesh->mName.C_Str() << std::endl;

		// 确保有足够的空间存储新网格的数据
		s_data.model_List.resize(numofmesh + 1);
		s_data.model_indices.resize(numofmesh + 1);

		// 清空当前网格的顶点和索引，以便存储新数据
		s_data.model_List[numofmesh].clear();
		s_data.model_indices[numofmesh].clear();

		for (unsigned int v = 0; v < mesh->mNumVertices; v++) {
			// 顶点位置
			s_data.model_List[numofmesh].push_back(glm::vec3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z));
		}

		for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
			const aiFace& face = mesh->mFaces[f];
			// 我们需要所有面都是三角形，所以一个面应该只有3个索引
			if (face.mNumIndices != 3) {
				std::cerr << "Warning: Mesh face with not exactly 3 indices, this is not supported." << std::endl;
				continue;
			}
			// 将面的所有索引添加到indices vector中
			for (unsigned int i = 0; i < face.mNumIndices; i++) {
				s_data.model_indices[numofmesh].push_back(face.mIndices[i]);
			}
		}

		numofmesh++;
	}

	// 递归处理每个子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Model_load::Model_load(std::string& path)
{
	Assimp::Importer importer;

	// 在这里替换为你的模型文件路径
	const std::string modelPath = path;

	auto start = std::chrono::high_resolution_clock::now();
	// 读取模型文件
	const aiScene* scene = importer.ReadFile(modelPath,
		aiProcess_Triangulate |       // 将所有模型的所有面都转换为三角形
		aiProcess_FlipUVs |           // 翻转y轴上的纹理坐标
		aiProcess_CalcTangentSpace |  // 计算切线和副切线
		aiProcess_GenNormals);        // 创建法线
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

	std::cout << "time" << duration << "s" << std::endl;

	// 处理Assimp的根节点
	ProcessNode(scene->mRootNode, scene);
}

Model_load::~Model_load()
{

}

std::tuple<std::vector<glm::vec3>, std::vector<unsigned int>> Model_load::GetVao(int i)
{
	return std::make_tuple(s_data.model_List[i], s_data.model_indices[i]);
}

