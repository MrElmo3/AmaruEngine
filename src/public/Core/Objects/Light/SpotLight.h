#pragma once
#include <Core/Objects/AObject.h>

class LightComponent;

class SpotLight : public AObject {
public:
	SpotLight(const std::string &_name, 
		AObject* _parent = nullptr, 
		ASceneController* _scene = nullptr);
	~SpotLight() override = default;

private:
	LightComponent* lightComponent;

public:

	SpotLight* SetColor(glm::vec3 color);
	SpotLight* SetColor(float r, float g, float b);
	SpotLight* SetRange(float range);
	SpotLight* SetIntensity(float intensity);
	SpotLight* SetSpotRange(float innerRange, float outerRnage);

	LightComponent* GetLightComponent() const { return lightComponent; }
};
