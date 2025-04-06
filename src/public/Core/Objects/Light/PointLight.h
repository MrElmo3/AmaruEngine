#pragma once
#include <Core/Objects/AObject.h>

class LightComponent;

class PointLight : public AObject {
public:
	PointLight(const std::string &_name, AObject* _parent = nullptr, ASceneController* _scene = nullptr);
	~PointLight() override = default;

private:
	LightComponent* lightComponent;

public:

	void SetRange(float range);
	void SetColor(glm::vec3 color);
	void SetColor(float r, float g, float b);
	void SetIntensity(float intensity);

	LightComponent* GetLightComponent() const { return lightComponent; }
};
