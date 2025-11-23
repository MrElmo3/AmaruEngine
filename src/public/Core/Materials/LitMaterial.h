#pragma once
#include <string>
#include <glm/glm.hpp>

#include "AMaterial.h"

class LitMaterial : public AMaterial {
private:
	glm::vec3 color = glm::vec3(1.0);
	unsigned int diffuse;
	glm::vec3 specular = glm::vec3(1.0);
	float shininess = 64.0f;

public:
	LitMaterial();
	~LitMaterial() override = default;

	void Use() override;

	LitMaterial* SetColor(float r, float g, float b, float a);
	LitMaterial* SetColor(glm::vec3 color);
	LitMaterial* SetDiffuse(const std::string &texturePath);
	LitMaterial* SetSpecular(glm::vec3 specular);
	LitMaterial* SetShininess(float shininess);

	glm::vec3 GetColor() const { return this->color; };
	unsigned int GetDiffuseTextureId() const { return this->diffuse; };
	glm::vec3 GetSpecular() const { return this->specular; };
	float GetShininess() const { return this->shininess; } ;

};
