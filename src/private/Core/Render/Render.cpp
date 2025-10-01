#include "Core/Render/Render.h"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb_image.h>
#include <Core/Global.h>
#include <Core/Window.h>
#include <Core/Materials/ColorMaterial.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Shader.h>
#include <Core/Materials/AMaterial.h>
#include <Core/Components/Render/CameraComponent.h>
#include <glm/gtc/matrix_transform.hpp>
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
	glGenVertexArrays(1, &VAOLine);

	glBindVertexArray(VAOLine);

	glGenBuffers(1, &VBOLine);
	glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Render::InitQuad() {

	glGenVertexArrays(1, &VAOQuad);
	glBindVertexArray(VAOQuad);

	//position buffer
	std::vector<glm::vec3> verticesPos =  {
		{-0.5f,	-0.5f,	0.0f},
		{ 0.5f,	-0.5f,	0.0f},
		{ 0.5f,	0.5f,	0.0f},
		{-0.5f,	0.5f,	0.0f}
	};
	glGenBuffers(1, &VBOQuadPos);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadPos);
	glBufferData(
		GL_ARRAY_BUFFER, 
		verticesPos.size() * sizeof(glm::vec3), 
		verticesPos.data(), 
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &VBOQuadUv);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadUv);
	glBufferData(
		GL_ARRAY_BUFFER, 
		4 * sizeof(glm::vec2), 
		nullptr, 
		GL_DYNAMIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//EBO
	std::vector<unsigned int> indices = {
		0, 1, 3,
		1, 2, 3
	};
	glGenBuffers(1, &EBOQuad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOQuad);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		indices.size() * sizeof(unsigned int), 
		indices.data(), 
		GL_STATIC_DRAW
	);
}

void Render::InitCube() {

	//VAO
	glGenVertexArrays(1, &VAOCube);
	glBindVertexArray(VAOCube);
	
	//VBO POS
	std::vector<glm::vec3> verticesPos = {

		//right face
		{+0.5f,	-0.5f,	+0.5f},
		{+0.5f,	-0.5f,	-0.5f},
		{+0.5f,	+0.5f,	-0.5f},
		{+0.5f,	+0.5f,	+0.5f},

		//left face
		{-0.5f,	-0.5f,	-0.5f},
		{-0.5f,	-0.5f,	+0.5f},
		{-0.5f,	+0.5f,	+0.5f},
		{-0.5f,	+0.5f,	-0.5f},

		//top face
		{-0.5f,	+0.5f,	+0.5f},
		{+0.5f,	+0.5f,	+0.5f},
		{+0.5f,	+0.5f,	-0.5f},
		{-0.5f,	+0.5f,	-0.5f},

		//bottom face
		{-0.5f,	-0.5f,	-0.5f},
		{+0.5f,	-0.5f,	-0.5f},
		{+0.5f,	-0.5f,	+0.5f},
		{-0.5f,	-0.5f,	+0.5f},

		//front face
		{-0.5f,	-0.5f,	+0.5f},
		{+0.5f,	-0.5f,	+0.5f},
		{+0.5f,	+0.5f,	+0.5f},
		{-0.5f,	+0.5f,	+0.5f},

		//back face
		{-0.5f,	-0.5f,	-0.5f},
		{+0.5f,	-0.5f,	-0.5f},
		{+0.5f,	+0.5f,	-0.5f},
		{-0.5f,	+0.5f,	-0.5f},
	};
	
	glGenBuffers(1, &VBOCubePos);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubePos);
	glBufferData(
		GL_ARRAY_BUFFER, 
		verticesPos.size() * sizeof(glm::vec3), 
		verticesPos.data(), 
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//VBO UV
	glGenBuffers(1, &VBOQuadUv);
	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadUv);
	glBufferData(
		GL_ARRAY_BUFFER, 
		24 * sizeof(glm::vec2), 
		nullptr, 
		GL_DYNAMIC_DRAW
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//VBO NORMALS
	std::vector<glm::vec3> verticesNorm = {
		//right face
		{+1.0f,	-0.0f,	+0.0f},
		{+1.0f,	-0.0f,	-0.0f},
		{+1.0f,	+0.0f,	-0.0f},
		{+1.0f,	+0.0f,	+0.0f},

		//left face
		{-1.0f,	-0.0f,	-0.0f},
		{-1.0f,	-0.0f,	+0.0f},
		{-1.0f,	+0.0f,	+0.0f},
		{-1.0f,	+0.0f,	-0.0f},

		//top face
		{-0.0f,	+1.0f,	+0.0f},
		{+0.0f,	+1.0f,	+0.0f},
		{+0.0f,	+1.0f,	-0.0f},
		{-0.0f,	+1.0f,	-0.0f},

		//bottom face
		{-0.0f,	-1.0f,	-0.0f},
		{+0.0f,	-1.0f,	-0.0f},
		{+0.0f,	-1.0f,	+0.0f},
		{-0.0f,	-1.0f,	+0.0f},

		//front face
		{-0.0f,	-0.0f,	+1.0f},
		{+0.0f,	-0.0f,	+1.0f},
		{+0.0f,	+0.0f,	+1.0f},
		{-0.0f,	+0.0f,	+1.0f},

		//back face
		{-0.0f,	-0.0f,	-1.0f},
		{+0.0f,	-0.0f,	-1.0f},
		{+0.0f,	+0.0f,	-1.0f},
		{-0.0f,	+0.0f,	-1.0f},
	};

	glGenBuffers(1, &VBOCubeNorm);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCubeNorm);
	glBufferData(
		GL_ARRAY_BUFFER,
		verticesNorm.size() * sizeof(glm::vec3), 
		verticesNorm.data(),
		GL_STATIC_DRAW
	);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//EBO
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

	glGenBuffers(1, &EBOCube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		indices.size() * sizeof(unsigned int), 
		indices.data(), 
		GL_STATIC_DRAW
	);
}

void Render::DrawLineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color) {
	if (currentCamera == nullptr) return;
	
	gizmosMaterial->SetColor(color);

	gizmosMaterial->Use();
	
	gizmosMaterial->shader->SetMatrix4("_model", glm::mat4(1.0f));
	gizmosMaterial->shader->SetMatrix4("_view", currentCamera->GetViewMatrix());
	gizmosMaterial->shader->SetMatrix4("_projection", currentCamera->GetProjectionMatrix());

	glLineWidth(1);

	glBindVertexArray(VAOLine);

	std::vector<float> vertices = {
		(float)start.x, (float)start.y, (float)start.z,
		(float)end.x, (float)end.y, (float)end.z
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBOLine);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		0, 
		vertices.size() * sizeof(glm::vec2), 
		vertices.data()
	);

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

void Render::DrawQuad(
	glm::mat4* model, 
	AMaterial* material, 
	std::vector<glm::vec2>* uv) {

	if (currentCamera == nullptr) return;
	
	material->Use();
	material->shader->SetMatrix4("_model", *model);
	material->shader->SetMatrix4("_view", currentCamera->GetViewMatrix());
	material->shader->SetMatrix4("_projection", currentCamera->GetProjectionMatrix());

	glBindVertexArray(VAOQuad);

	if(uv == nullptr || uv->size() != 4) {
		uv = new std::vector<glm::vec2>({
			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},
		});
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadUv);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		0, 
		uv->size() * sizeof(float), 
		uv->data()
	);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Render::DrawCube(
	glm::mat4* model, 
	AMaterial* material,
	std::vector<glm::vec2>* uv) {

	if (!currentCamera) return;

	LightComponent* sceneLight = Window::GetInstance().GetActualScene()->lightSource;
	if (!sceneLight) return;

	material->Use();
	
	material->shader->SetMatrix4("_model", *model);
	material->shader->SetMatrix4("_view", currentCamera->GetViewMatrix());
	material->shader->SetMatrix4("_projection", currentCamera->GetProjectionMatrix());

	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(*model)));
	material->shader->SetMatrix3("_normalModel", normalMatrix);

	material->shader->SetVector3("_viewPosition", currentCamera->parent->GetWorldPosition());

	material->shader->SetFloat("_ambientStrength", Global::AMBIENT_LIGHT_STRENGTH);
	material->shader->SetFloat("_specularStrength", Global::SPECULAR_LIGHT_STRENGTH);
	material->shader->SetUInt("_shininess", Global::SHININESS);

	material->shader->SetVector3("_lightColor", sceneLight->GetColor());
	material->shader->SetVector3("_lightPosition", sceneLight->GetPosition());
	material->shader->SetFloat("_lightRange", sceneLight->GetRange());
	material->shader->SetFloat("_lightIntensity", sceneLight->GetIntensity());

	glBindVertexArray(VAOCube);

	if(uv == nullptr || uv->size() != 24) {
		uv = new std::vector<glm::vec2>({
			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},

			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},

			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},

			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},

			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},

			{0.0f,	0.0f},
			{1.0f,	0.0f},
			{1.0f,	1.0f},
			{0.0f,	1.0f},
		});
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOQuadUv);
	glBufferSubData(
		GL_ARRAY_BUFFER, 
		0, 
		uv->size() * sizeof(glm::vec2), 
		uv->data()
	);

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