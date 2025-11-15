#pragma once
#include <Core/Components/IComponent.h>
#include <glm/glm.hpp>

class Shader;

enum class LightType {
	Point = 0,
};

class LightComponent : public IComponent {
private:

	LightType type;

	glm::vec3 lightPosition;
	glm::vec3 color = glm::vec3(1.0);
	glm::vec3 ambient = glm::vec3(0.2);
	glm::vec3 diffuse = glm::vec3(0.5);
	glm::vec3 specular = glm::vec3(1.0);
	float range = 1.0f;
	float intensity = 1.0f;

public:
	explicit LightComponent(AObject* _parent);
	~LightComponent() override = default;

	void Update(double deltaTime) override;

	void Use(Shader* shader);

	void SetColor(glm::vec3 color);
	void SetColor(float r, float g, float b);
	void SetIntensity(float intensity);
	void SetRange(float range);

	glm::vec3 GetColor() const { return this->color; }
	glm::vec3 GetPosition() const { return this->lightPosition; }
	float GetRange() const { return this->range; }
	float GetIntensity() const { return this->intensity; }
};

