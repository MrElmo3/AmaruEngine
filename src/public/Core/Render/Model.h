#pragma once
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;
class Model {
public:
	Model(std::string modelPath);
	void Draw();

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(std::string modelPath);
	void ProcessNode(aiNode *node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene* scene);
};