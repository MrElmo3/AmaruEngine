#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <core/Render/Shader.h>

struct Vertex;

class Mesh {
public:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(
		std::vector<Vertex> vertices, 
		std::vector<unsigned int> indices 
	);
    
	void Draw(Shader &shader);

private:
	//render data
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
};

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};