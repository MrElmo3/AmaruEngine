#include "Core/Render/Render.h"

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <Core/Window.h>
#include <Core/Materials/ColorMaterial.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Shader.h>
#include <Core/Materials/AMaterial.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Util/Logger.h>


Render::Render() {

	gizmosMaterial = new ColorMaterial();

	InitQuad();
	InitLine();
	InitCube();
}

Render::~Render() {

}

Shader* Render::CreateShader(const std::string vertexPath, const std::string fragmentPath) {

	for (Shader* shader : shaders) {
		if (shader->CompareFiles(vertexPath, fragmentPath)) {
			return shader;
		}
	}

	Shader* newShader = new Shader(vertexPath, fragmentPath);
	shaders.push_back(newShader);
	return newShader;
}

unsigned int Render::GenerateTexture(const std::string texturePath) {

	if (textures.find(texturePath) != textures.end()) {
		return textures[texturePath];
	}
	
	unsigned int generatedTexture;

	glGenTextures(1, &generatedTexture);
	glBindTexture(GL_TEXTURE_2D, generatedTexture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		Logger::Error("Failed to load texture");
		return 0;
	}
	stbi_image_free(data);

	textures[texturePath] = generatedTexture;

	glBindTexture(GL_TEXTURE_2D, 0);
	
	return generatedTexture;
}

void Render::InitLine() {
	glGenVertexArrays(1, &VAO_line);

	glBindVertexArray(VAO_line);

	glGenBuffers(1, &VBO_line);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Render::InitQuad() {

	std::vector<Vertex> vertices =  {
		{{-0.5f,	-0.5f,	0.0f}, {0.0f,	0.0f}},
		{{ 0.5f,	-0.5f,	0.0f}, {1.0f,	0.0f}},
		{{ 0.5f,	0.5f,	0.0f}, {1.0f,	1.0f}},
		{{-0.5f,	0.5f,	0.0f}, {0.0f,	1.0f}}
	};

	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO_quad);

	glBindVertexArray(VAO_quad);

	glGenBuffers(1, &VBO_quad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
	glBufferData(
		GL_ARRAY_BUFFER, 
		vertices.size() * sizeof(Vertex), 
		vertices.data(), 
		GL_STATIC_DRAW
	);

	glGenBuffers(1, &EBO_quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_quad);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		indices.size() * sizeof(unsigned int), 
		indices.data(), 
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void Render::InitCube() {
	
	std::vector<Vertex> vertices = {

		//right face
		{{+0.5f,	-0.5f,	+0.5f}, {0.0f,	0.0f}},
		{{+0.5f,	-0.5f,	-0.5f}, {1.0f,	0.0f}},
		{{+0.5f,	+0.5f,	-0.5f}, {1.0f,	1.0f}},
		{{+0.5f,	+0.5f,	+0.5f}, {0.0f,	1.0f}},

		//left face
		{{-0.5f,	-0.5f,	-0.5f}, {0.0f,	0.0f}},
		{{-0.5f,	-0.5f,	+0.5f}, {1.0f,	0.0f}},
		{{-0.5f,	+0.5f,	+0.5f}, {1.0f,	1.0f}},
		{{-0.5f,	+0.5f,	-0.5f}, {0.0f,	1.0f}},

		//top face
		{{-0.5f,	+0.5f,	+0.5f}, {0.0f,	0.0f}},
		{{+0.5f,	+0.5f,	+0.5f}, {1.0f,	0.0f}},
		{{+0.5f,	+0.5f,	-0.5f}, {1.0f,	1.0f}},
		{{-0.5f,	+0.5f,	-0.5f}, {0.0f,	1.0f}},

		//bottom face
		{{-0.5f,	-0.5f,	-0.5f}, {0.0f,	0.0f}},
		{{+0.5f,	-0.5f,	-0.5f}, {1.0f,	0.0f}},
		{{+0.5f,	-0.5f,	+0.5f}, {1.0f,	1.0f}},
		{{-0.5f,	-0.5f,	+0.5f}, {0.0f,	1.0f}},

		//front face
		{{-0.5f,	-0.5f,	+0.5f}, {0.0f,	0.0f}},
		{{+0.5f,	-0.5f,	+0.5f}, {1.0f,	0.0f}},
		{{+0.5f,	+0.5f,	+0.5f}, {1.0f,	1.0f}},
		{{-0.5f,	+0.5f,	+0.5f}, {0.0f,	1.0f}},

		//back face
		{{-0.5f,	-0.5f,	-0.5f}, {0.0f,	0.0f}},
		{{+0.5f,	-0.5f,	-0.5f}, {1.0f,	0.0f}},
		{{+0.5f,	+0.5f,	-0.5f}, {1.0f,	1.0f}},
		{{-0.5f,	+0.5f,	-0.5f}, {0.0f,	1.0f}},
	};

	std::vector<unsigned int> indices = {
		//right face
		0, 1, 3,
		1, 2, 3,
		
		//left face
		4, 5, 7,
		5, 6, 7,

		//top face
		8, 9, 11,
		9, 10, 11,

		//bottom face
		12, 13, 15,
		13, 14, 15,

		//front face
		16, 17, 19,
		17, 18, 19,

		//back face
		20, 21, 23,
		21, 22, 23
	};

	glGenVertexArrays(1, &VAO_cube);

	glBindVertexArray(VAO_cube);

	glGenBuffers(1, &VBO_cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_cube);
	glBufferData(GL_ARRAY_BUFFER, 
		vertices.size() * sizeof(Vertex), 
		vertices.data(), 
		GL_STATIC_DRAW);

	glGenBuffers(1, &EBO_cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		indices.size() * sizeof(unsigned int), 
		indices.data(), 
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Render::DrawLineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
	if (currentCamera == nullptr) return;
	
	gizmosMaterial->SetColor(color);

	gizmosMaterial->Use();
	
	gizmosMaterial->shader->SetMatrix4("_model", glm::mat4(1.0f));
	gizmosMaterial->shader->SetMatrix4("_view", currentCamera->GetViewMatrix());
	gizmosMaterial->shader->SetMatrix4("_projection", currentCamera->GetProjectionMatrix());

	float vertices[] = {
		(float)start.x, (float)start.y, (float)start.z,
		(float)end.x, (float)end.y, (float)end.z
	};

	glLineWidth(1);

	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), vertices);

	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
}

void Render::DrawQuadLine(glm::vec2 center, glm::vec2 scale, glm::vec3 color) {

	glm::vec3 points[] = {
		glm::vec3(center.x - scale.x / 2, center.y - scale.y / 2, 0), //bottom left
		glm::vec3(center.x + scale.x / 2, center.y - scale.y / 2, 0), //bottom right
		glm::vec3(center.x + scale.x / 2, center.y + scale.y / 2, 0), //top right
		glm::vec3(center.x - scale.x / 2, center.y + scale.y / 2, 0), //top left
	};

	DrawLineSegment(points[0], points[1], color);
	DrawLineSegment(points[1], points[2], color);
	DrawLineSegment(points[2], points[3], color);
	DrawLineSegment(points[3], points[0], color);

}

void Render::DrawQuad(glm::mat4 model, AMaterial* material) {
	if (currentCamera == nullptr) return;
	
	material->Use();
	material->shader->SetMatrix4("_model", model);
	material->shader->SetMatrix4("_view", currentCamera->GetViewMatrix());
	material->shader->SetMatrix4("_projection", currentCamera->GetProjectionMatrix());

	glBindVertexArray(VAO_quad);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Render::DrawCube(glm::mat4 model, AMaterial* material) {
	if (!currentCamera) return;
	if (!Window::GetInstance().GetActualScene()->lightSource) return;

	material->Use();
	
	material->shader->SetMatrix4("_model", model);
	material->shader->SetMatrix4("_view", currentCamera->GetViewMatrix());
	material->shader->SetMatrix4("_projection", currentCamera->GetProjectionMatrix());

	material->shader->SetVector3("_lightColor",
		Window::GetInstance().GetActualScene()->lightSource->GetColor());

	glBindVertexArray(VAO_cube);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

glm::mat4 Render::GetModelMatrix(AObject* object){

	glm::mat4 model = glm::mat4(1);

	glm::quat rotation = object->GetWorldRotation();
	glm::vec3 vectorRotation = glm::vec3(rotation.x, rotation.y, rotation.z);
	float angle = 2 * glm::acos(rotation.w);
	
	model = glm::translate(model, object->GetWorldPosition());
	if (vectorRotation != glm::vec3(0))
		model = glm::rotate(model, angle, vectorRotation);
	model = glm::scale(model, object->GetWorldScale());

	return model;
}

void Render::SetCurrentCamera(CameraComponent* camera) {
	currentCamera = camera;
}