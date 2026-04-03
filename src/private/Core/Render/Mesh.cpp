#include <Core/Render/Mesh.h>
#include <Core/Materials/AMaterial.h>

Mesh::Mesh(
	std::vector<Vertex> vertices, 
	std::vector<unsigned int> indices,
	glm::mat4x4 meshTransform) {
	
	this->vertices = vertices;
	this->indices = indices;
	this->meshTransform = meshTransform;
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
		&vertices[0],
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

void Mesh::Draw(AMaterial* material, glm::mat4x4* parentTransform) {

	glm::mat4x4 transform = *parentTransform * meshTransform;

	material->shader->SetMatrix4("_model", transform);

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(transform)));
	material->shader->SetMatrix3("_normalModel", normalMatrix);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}