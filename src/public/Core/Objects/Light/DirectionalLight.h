#pragma once
#include <Core/Objects/AObject.h>

class LightComponent;

class DirectionalLight : public AObject {
public:
	DirectionalLight(const std::string &_name, 
		AObject* _parent = nullptr, 
		ASceneController* _scene = nullptr);
	~DirectionalLight() override = default;

private:
	LightComponent* lightComponent;

public:

	DirectionalLight* SetColor(glm::vec3 color);
	DirectionalLight* SetColor(float r, float g, float b);
	DirectionalLight* SetIntensity(float intensity);

	LightComponent* GetLightComponent() const { return lightComponent; }
};
