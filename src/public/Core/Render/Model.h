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

	/// @brief Axis aligned bounds of the whole model in its own space (mesh transforms applied).
	/// Useful to fit a model of unknown size into the scene.
	void GetBounds(glm::vec3& min, glm::vec3& max) const;

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void LoadModel(std::string modelPath);
	void ProcessNode(aiNode *node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh *mesh, glm::mat4x4 transform);
};