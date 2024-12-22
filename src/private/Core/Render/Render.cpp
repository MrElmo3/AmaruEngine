#include "Core/Render/Render.h"

#include <glad/glad.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Render/Shader.h"
#include "Core/Materials/GizmosMaterial.h"
#include "Core/Global.h"
#include "Core/Render/TextRender.h"

Render::Render() {

	gizmosMaterial = new GizmosMaterial();

	InitQuad();
	InitLine();
	InitText();
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

glm::vec3 Render::TransformWorldToScreen(glm::vec3 worldScale) {
	return glm::vec3(
		(Global::METER / Global::SCREEN_FORMAT) * worldScale.x,
		Global::METER * worldScale.y, 0);
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

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO_quad);

	glBindVertexArray(VAO_quad);

	glGenBuffers(1, &VBO_quad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &EBO_quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void Render::InitText() {

	std::string vertexPath =	"Assets/Shaders/BaseShader/BaseVert.glsl";
	std::string fragmentPath =	"Assets/Shaders/BaseShader/BaseFrag.glsl";

	textShader = CreateShader(vertexPath, fragmentPath);
	
	TextRender::Init();
	
	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &VAO_text);

	glBindVertexArray(VAO_text);
	
	glGenBuffers(1, &VBO_text);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &EBO_quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Render::DrawLineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color) {

	start = TransformWorldToScreen(start);
	end = TransformWorldToScreen(end);

	gizmosMaterial->SetColor(color);

	gizmosMaterial->shader->Use();
	gizmosMaterial->SetColorUniform();
	gizmosMaterial->shader->SetMatrix4("_transform", glm::mat4(1.0f));

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

void Render::DrawQuad(glm::vec3 center, glm::vec3 scale, Shader* shader, glm::vec3 color) {
	
	glm::vec3 scaleScreen = TransformWorldToScreen(scale);
	glm::vec3 centerScreen = TransformWorldToScreen(center);
	
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, centerScreen);
	trans = glm::scale(trans, scaleScreen);

	shader->Use();
	shader->SetVector3("_color", color);
	shader->SetMatrix4("_transform", trans);

	glBindVertexArray(VAO_quad);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Render::DrawQuadLine(glm::vec3 center, glm::vec3 scale, glm::vec3 color) {

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

void Render::DrawText(std::string text, glm::vec3 position, float scale, glm::vec3 color) {
	textShader->Use();
	textShader->SetVector3("_textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO_text);
	
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) 
	{
		Character ch = TextRender::Characters[*c];

		float xpos = position.x + ch.Bearing.x * scale;
		float ypos = position.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		float vertices[4][5] = {
			{ xpos,     ypos + h,	0.0f,	0.0f,	0.0f },            
			{ xpos,     ypos,		0.0f,	0.0f,	1.0f },
			{ xpos + w, ypos,		0.0f,	1.0f,	1.0f },
			{ xpos + w, ypos + h,	0.0f,	1.0f,	0.0f }           
		};
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_text);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		position.x += (ch.Advance >> 6) * scale; 
	}
}