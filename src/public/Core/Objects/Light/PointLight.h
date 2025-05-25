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
	LightComponent* GetLightComponent() const { return lightComponent; }
};
