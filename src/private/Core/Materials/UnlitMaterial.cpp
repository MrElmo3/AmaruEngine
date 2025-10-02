#include <string>
#include <Core/Materials/UnlitMaterial.h>
#include <Core/Render/Render.h>
#include <Core/Render/Shader.h>

UnlitMaterial::UnlitMaterial() {
	texture = Render::GetInstance().GenerateTexture("Assets/Textures/Default.png");

	std::string vertexPath =	"Assets/Shaders/UnlitShader/Unlit.vert";
	std::string fragmentPath =	"Assets/Shaders/UnlitShader/Unlit.frag";
	this->shader = Render::GetInstance().CreateShader(vertexPath, fragmentPath);
}

void UnlitMaterial::Use() {
	this->shader->Use();
	this->shader->SetVector3("_color", this->color);
	this->shader->SetTexture(texture, 0);
}

UnlitMaterial* UnlitMaterial::SetColor(float r, float g, float b, float a) {
	this->color = glm::vec4(r, g, b, a);
	return this;
}

UnlitMaterial* UnlitMaterial::SetColor(const glm::vec3 color) {
	this->color = color;
	return this;
}

UnlitMaterial* UnlitMaterial::SetTexture(const std::string &texturePath) {
	this->texture = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}