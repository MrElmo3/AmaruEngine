#pragma once
#include <Core/Components/IComponent.h>
#include <glm/glm.hpp>


enum LightType {
	Point = 0,
};

class LightComponent : public IComponent {
private:

	LightType type;

	glm::vec3 lightPosition;
	float range;
	glm::vec3 color;
	float intensity;

public:
	explicit LightComponent(AObject* _parent);
	~LightComponent() override = default;

	void Update(double deltaTime) override;

	void SetColor(glm::vec3 color);
	void SetColor(float r, float g, float b);
	void SetIntensity(float intensity);
	void SetRange(float range);

	glm::vec3 GetLightPosition() const { return this->lightPosition; }
	glm::vec3 GetColor() const { return this->color; }
	float GetIntensity() const { return this->intensity; }
};

