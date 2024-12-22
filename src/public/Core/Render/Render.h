#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Util/Singleton.h"

class Shader;
class GizmosMaterial;

class Render : public Singleton<Render> {
private:
	std::vector<Shader*> shaders;
	unsigned int VAO_quad;
	unsigned int VBO_quad;
	unsigned int EBO_quad;
	
	unsigned int VAO_line;
	unsigned int VBO_line;

	unsigned int VAO_text;
	unsigned int VBO_text;
	unsigned int EBO_text;

	GizmosMaterial* gizmosMaterial;
	Shader* textShader;

public:
	Render();
	~Render();

private:
	void InitQuad();
	void InitLine();
	void InitText();

	glm::vec3 TransformWorldToScreen(glm::vec3 worldScale);

public:
	Shader* CreateShader(const std::string vertexPath, const std::string fragmentPath);
	void DrawQuad(glm::vec3 center, glm::vec3 size, Shader* shader, glm::vec3 color = glm::vec3(1.0f));
	void DrawLineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color);
	void DrawQuadLine(glm::vec3 center, glm::vec3 size, glm::vec3 color);
	void DrawText(std::string text, glm::vec3 position, float scale, glm::vec3 color);
};

template class Singleton<Render>;