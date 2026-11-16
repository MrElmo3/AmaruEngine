#pragma once
#include <string>
#include <glm/glm.hpp>

#include "AMaterial.h"

class LitMaterial : public AMaterial {
private:
	glm::vec3 color = glm::vec3(1.0);
	unsigned int diffuse = 0;
	unsigned int specular = 0;
	unsigned int emission = 0;
	float shininess = 64.0f;

	//PBR stuff
	bool isPBR = false;
	unsigned int albedo = 0;
	unsigned int normal = 0;
	unsigned int metallic = 0;
	unsigned int roughness = 0;
	unsigned int ao = 0;

public:
	LitMaterial();
	~LitMaterial() override = default;

	void Use() override;

	LitMaterial* SetColor(float r, float g, float b, float a);
	LitMaterial* SetColor(glm::vec3 color);
	LitMaterial* SetDiffuse(const std::string &texturePath);
	LitMaterial* SetSpecular(const std::string &texturePath);
	LitMaterial* SetEmission(const std::string &texturePath);
	LitMaterial* SetShininess(float shininess);

	LitMaterial* IsPBR(const bool value);
	LitMaterial* SetAlbedo(const std::string &texturePath);
	LitMaterial* SetNormal(const std::string &texturePath);
	LitMaterial* SetMetallic(const std::string &texturePath);
	LitMaterial* SetRoughness(const std::string &texturePath);
	LitMaterial* SetAO(const std::string &texturePath);

	glm::vec3 GetColor() const { return this->color; };
	unsigned int GetDiffuseTextureId() const { return this->diffuse; };
	unsigned int GetSpecularTextureId() const { return this->specular; };
	unsigned int GetEmissionTextureId() const { return this->emission; };
	float GetShininess() const { return this->shininess; } ;

	unsigned int GetAlbedoTextureId() const { return this->albedo; };
	unsigned int GetNormalTextureId() const { return this->normal; };
	unsigned int GetMetallicTextureId() const { return this->metallic; };
	unsigned int GetRoughnessTextureId() const { return this->roughness; };
	unsigned int GetAOTextureId() const { return this->ao; };
};
