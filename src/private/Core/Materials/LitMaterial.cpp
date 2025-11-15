#include <Core/Materials/LitMaterial.h>
#include <Core/Render/Render.h>
#include <Core/Render/Shader.h>

LitMaterial::LitMaterial() {
	texture = Render::GetInstance().GenerateTexture("Assets/Textures/Default.png");

	std::string vertexPath =	"Assets/Shaders/LitShader/Lit.vert";
	std::string fragmentPath =	"Assets/Shaders/LitShader/Lit.frag";
	this->shader = Render::GetInstance().CreateShader(vertexPath, fragmentPath);
}

void LitMaterial::Use() {
	this->shader->Use();
	this->shader->SetVector3("_material.color", this->color);
	this->shader->SetTexture(texture, 0);

	this->shader->SetVector3("_material.ambient", this->ambient);
	this->shader->SetVector3("_material.diffuse", this->diffuse);
	this->shader->SetVector3("_material.specular", this->specular);
	this->shader->SetFloat("_material.shininess", this->shininess);
}

LitMaterial* LitMaterial::SetColor(float r, float g, float b, float a) {
	this->color = glm::vec4(r, g, b, a);
	return this;
}

LitMaterial* LitMaterial::SetColor(glm::vec3 color) {
	this->color = color;
	return this;
}

LitMaterial* LitMaterial::SetAmbient(const glm::vec3 ambient) {
	this->ambient = ambient;
	return this;
}

LitMaterial* LitMaterial::SetDiffuse(const glm::vec3 diffuse) {
	this->diffuse = diffuse;
	return this;
}

LitMaterial* LitMaterial::SetSpecular(const glm::vec3 specular) {
	this->specular = specular;
	return this;
}

LitMaterial* LitMaterial::SetShininess(const float shininess) {
	this->shininess= shininess;
	return this;
}

LitMaterial* LitMaterial::SetTexture(const std::string &texturePath) {
	this->texture = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}
