#include "Core/Materials/GizmosMaterial.h"
#include "Core/Render/Shader.h"
#include <string>

#include "Core/Render/Render.h"

GizmosMaterial::GizmosMaterial() {
	std::string vertexPath = "Assets/Shaders/ColorShader/ColorVert.glsl";
	std::string fragmentPath = "Assets/Shaders/ColorShader/ColorFrag.glsl";
	this->shader = new Shader(vertexPath, fragmentPath);
}

GizmosMaterial::~GizmosMaterial() {
	delete this->shader;
}

void GizmosMaterial::SetColor(float r, float g, float b) {
	this->color = glm::vec3(r, g, b);
}

void GizmosMaterial::SetColor(glm::vec3 color) {
	this->color = color;
}

void GizmosMaterial::SetColorUniform() {
	this->shader->Use();
	this->shader->SetVector3("_color", this->color);
}

