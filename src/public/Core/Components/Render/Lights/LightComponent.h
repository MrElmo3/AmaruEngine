#pragma once
#include <Core/Components/IComponent.h>
#include <Core/Render/LightData.h>
#include <glm/glm.hpp>

class Shader;

class LightComponent : public IComponent {
private:

	LightData data;
	float range;

public:
	LightComponent(AObject* _parent);
	~LightComponent() override = default;

	void LateUpdate() override;

	LightData GetLightData();

	LightComponent* SetType(LightType _type);
	LightComponent* SetColor(glm::vec3 _color);
	LightComponent* SetColor(float r, float g, float b);
	LightComponent* SetIntensity(float _intensity);
	LightComponent* SetRange(float _range);
	LightComponent* SetSpotRange(float _inner, float outer);
};



