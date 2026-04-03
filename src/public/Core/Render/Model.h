#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/mat4x4.hpp>

class AMaterial;
class Mesh;
class Model {
public:
	Model(std::string modelPath);
	void Draw(AMaterial* material, glm::mat4x4* parentTransform);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(std::string modelPath);
	void ProcessNode(aiNode *node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, glm::mat4x4 transform);
};