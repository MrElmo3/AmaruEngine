#pragma once
#include <string>
#include <glm/glm.hpp>

#include "AMaterial.h"

class LitMaterial : public AMaterial {
private:
	unsigned int texture;
	glm::vec3 color = glm::vec3(1.0);
	glm::vec3 ambient = glm::vec3(1.0);
	glm::vec3 diffuse = glm::vec3(1.0);
	glm::vec3 specular = glm::vec3(1.0);
	float shininess = 64.0f;

public:
	LitMaterial();
	~LitMaterial() override = default;

	void Use() override;

	LitMaterial* SetColor(float r, float g, float b, float a);
	LitMaterial* SetColor(glm::vec3 color);
	LitMaterial* SetAmbient(glm::vec3 ambient);
	LitMaterial* SetDiffuse(glm::vec3 difusse);
	LitMaterial* SetSpecular(glm::vec3 specular);
	LitMaterial* SetShininess(float shininess);
	LitMaterial* SetTexture(const std::string &texturePath);

	glm::vec3 GetColor() const { return this->color; };
	glm::vec3 GetAmbient() const { return this->ambient; };
	glm::vec3 GetDiffuse() const { return this->diffuse; };
	glm::vec3 GetSpecular() const { return this->specular; };
	float GetShininess() const { return this->shininess; } ;

};
