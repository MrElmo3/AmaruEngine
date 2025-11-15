#pragma once
#include "AMaterial.h"
#include <glm/glm.hpp>

class UnlitMaterial : public AMaterial {
private:
	unsigned int texture;
	glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);

public:
	UnlitMaterial();
	~UnlitMaterial() override = default;

	void Use() override;

	UnlitMaterial* SetColor(float r, float g, float b, float a);
	UnlitMaterial* SetColor(glm::vec3 color);
	UnlitMaterial* SetTexture(const std::string &texturePath);

	glm::vec3 GetColor() const { return this->color; };
};