#include <Core/Materials/LitMaterial.h>
#include <Core/Render/Render.h>
#include <Core/Render/Shader.h>

LitMaterial::LitMaterial() {
	diffuse = Render::GetInstance().GenerateTexture("Assets/Textures/Default.png");

	std::string vertexPath =	"Assets/Shaders/LitShader/Lit.vert";
	std::string fragmentPath =	"Assets/Shaders/LitShader/Lit.frag";
	this->shader = Render::GetInstance().CreateShader(vertexPath, fragmentPath);
}

void LitMaterial::Use() {
	if(isPBR){
		this->shader->Use();
		this->shader->SetVector3("_material.color", this->color);
		this->shader->SetInt("_material.albedoMap", 0);
		this->shader->SetInt("_material.normalMap", 1);
		this->shader->SetTexture(albedo, 0);
		this->shader->SetTexture(normal, 1);
		return;
	}

	this->shader->Use();
	this->shader->SetVector3("_material.color", this->color);
	this->shader->SetInt("_material.diffuse", 0);
	this->shader->SetInt("_material.specular", 1);
	this->shader->SetInt("_material.emission", 2);
	this->shader->SetTexture(diffuse, 0);
	this->shader->SetTexture(specular, 1);
	this->shader->SetTexture(emission, 2);
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

LitMaterial* LitMaterial::SetDiffuse(const std::string &texturePath) {
	this->diffuse = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetSpecular(const std::string &texturePath) {
	this->specular = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetEmission(const std::string &texturePath) {
	this->emission = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetShininess(const float shininess) {
	this->shininess= shininess;
	return this;
}

LitMaterial* LitMaterial::IsPBR(const bool isPBR){
	this->isPBR = isPBR;
	return this;
}

LitMaterial* LitMaterial::SetAlbedo(const std::string &texturePath) {
	this->albedo = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetNormal(const std::string &texturePath) {
	this->normal = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetMetallic(const std::string &texturePath) {
	this->metallic= Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetRoughness(const std::string &texturePath) {
	this->roughness= Render::GetInstance().GenerateTexture(texturePath);
	return this;
}

LitMaterial* LitMaterial::SetAO(const std::string &texturePath) {
	this->ao = Render::GetInstance().GenerateTexture(texturePath);
	return this;
}
