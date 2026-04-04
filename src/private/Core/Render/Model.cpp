#include <Core/Render/Model.h>
#include <Core/Render/Mesh.h>
#include <Util/Logger.h>
#include <Util/Utility.h>
#include <Core/Materials/AMaterial.h>

Model::Model(std::string modelPath) {
	LoadModel(modelPath);
}

void Model::Draw(AMaterial* material, glm::mat4x4* parentTransform) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(material, parentTransform);
}

void Model::LoadModel(std::string modelPath) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(
		modelPath,
		aiProcess_Triangulate | 
		aiProcess_GlobalScale
	);

	if (!scene || 
		scene->mFlags & 
		AI_SCENE_FLAGS_INCOMPLETE || 
		!scene->mRootNode) {
		
		std::string error = importer.GetErrorString();
		Logger::Error("ERROR::ASSIMP::" + error);
		return;
	}
	directory = modelPath.substr(0, modelPath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene* scene) {
	
	glm::mat4x4 nodeTransform = Utility::ConvertMatrix4x4AssimpToGLM(node->mTransformation);
	glm::mat4x4 globalTransform = nodeTransform;
	
	for(unsigned int i = 0; i< node->mNumMeshes; i++){
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, nodeTransform));
	}
	
	for(unsigned int i = 0; i < node->mNumChildren; i++){
		ProcessNode(node->mChildren[i], scene);
	}
}


Mesh Model::ProcessMesh(aiMesh *mesh, const glm::mat4x4 transform) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	for(unsigned int i = 0; i < mesh->mNumVertices; i++){
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		if(mesh->HasNormals()){
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if(mesh->mTextureCoords[0]){
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0);
		}
		vertices.push_back(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++){
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++){
			indices.push_back(face.mIndices[j]);
		}
	}

	return Mesh(vertices, indices, transform);
}
