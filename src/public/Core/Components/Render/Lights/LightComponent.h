#pragma once
#include <Core/Components/IComponent.h>
#include <glm/glm.hpp>

class Shader;

enum class LightType {
	Directional = 0,
	Point = 1,
	Spot = 2,
};

class LightComponent : public IComponent {
private:

	LightType type;

	glm::vec3 lightPosition;
	glm::vec3 lightDirection;

	glm::vec3 color = glm::vec3(1.0);
	glm::vec3 ambient = glm::vec3(0.075);
	glm::vec3 diffuse = glm::vec3(0.4);
	glm::vec3 specular = glm::vec3(1.1);

	float intensity = 1.0f;
	
	//Point of light values
	float range;

	float linearValue = 0;
	float quadraticValue = 0;

	float innerRange;
	float outerRange;

public:
	explicit LightComponent(AObject* _parent);
	~LightComponent() override = default;

	void Update(double deltaTime) override;

	void Use(Shader* shader, int index = 0);

	LightComponent* SetType(LightType _type);
	LightComponent* SetColor(glm::vec3 _color);
	LightComponent* SetColor(float r, float g, float b);
	LightComponent* SetIntensity(float _intensity);
	LightComponent* SetRange(float _range);
	LightComponent* SetSpotRange(float _inner, float outer);

	LightType GetType() const { return this->type; }
	glm::vec3 GetPosition() const { return this->lightPosition; }
	glm::vec3 GetDirection() const { return this->lightDirection; }
	glm::vec3 GetColor() const { return this->color; }
	glm::vec3 GetAmbient() const { return this->ambient; }
	glm::vec3 GetDiffuse() const { return this->diffuse; }
	glm::vec3 GetSpecular() const { return this->specular; }
	float GetRange() const { return this->range; }
	float GetIntensity() const { return this->intensity; }
};

