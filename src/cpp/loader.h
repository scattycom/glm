#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <tuple>
#include<glm/glm.hpp>

class Model_load
{
public:
	Model_load(std::string& path);
	~Model_load();

	static std::tuple<std::vector<glm::vec3>, std::vector<unsigned int>> GetVao(int i);
	

};

class modelRender
{
public:

};

