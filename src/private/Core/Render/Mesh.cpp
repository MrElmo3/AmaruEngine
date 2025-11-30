#include <Core/Render/Mesh.h>

Mesh::Mesh(
	std::vector<Vertex> vertices, 
	std::vector<unsigned int> indices) {
	
	this->vertices = vertices;
	this->indices = indices;
	SetupMesh();
}

void Mesh::SetupMesh() {
	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(
		GL_ARRAY_BUFFER,
		vertices.size() * sizeof(Vertex),
		vertices.data(),
		GL_STATIC_DRAW
	);

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(unsigned int),
		indices.data(),
		GL_STATIC_DRAW
	);

	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), (void*)0
	);
	
	// vertex UV
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(
		1, 2, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)
	);

	// vertex Normal
	glEnableVertexAttribArray(2);	
    glVertexAttribPointer(
		2, 3, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), (void*)offsetof(Vertex, Normal)
	);

    glBindVertexArray(0);
}

void Mesh::Draw() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}