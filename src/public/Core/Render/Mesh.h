#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Render/Shader.h>

struct Vertex;
class AMaterial;
class Mesh {
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(
		std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices ,
		glm::mat4x4 meshTransform
	);
    
	void Draw(AMaterial* material, glm::mat4x4* parentTransform);

private:

	glm::mat4x4 meshTransform;

	//render data
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};