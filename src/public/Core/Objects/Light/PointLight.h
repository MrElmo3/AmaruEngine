#pragma once
#include <Core/Objects/AObject.h>

class LightComponent;

class PointLight : public AObject {
public:
	PointLight(const std::string &_name, 
		AObject* _parent = nullptr, 
		ASceneController* _scene = nullptr);
	~PointLight() override = default;

private:
	LightComponent* lightComponent;

public:

	PointLight* SetRange(float range);
	PointLight* SetColor(glm::vec3 color);
	PointLight* SetColor(float r, float g, float b);
	PointLight* SetIntensity(float intensity);

	LightComponent* GetLightComponent() const { return lightComponent; }
};
