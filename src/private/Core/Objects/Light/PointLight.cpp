#include <Core/Objects/Light/PointLight.h>
#include <Core/Global.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>

PointLight::PointLight(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {
	lightComponent = AddComponent<LightComponent>();
}

PointLight* PointLight::SetColor(glm::vec3 color) {
	lightComponent->SetColor(color);
	return this;
}

PointLight* PointLight::SetColor(float r, float g, float b) {
	lightComponent->SetColor(r, g, b);
	return this;
}

PointLight* PointLight::SetIntensity(float intensity) {
	lightComponent->SetIntensity(intensity);
	return this;
}

PointLight* PointLight::SetRange(float range) {
	lightComponent->SetRange(range);
	return this;
}

