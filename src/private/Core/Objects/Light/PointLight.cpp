#include <Core/Global.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Core/Objects/Light/PointLight.h>
#include <Util/Logger.h>

PointLight::PointLight(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {
	lightComponent = dynamic_cast<LightComponent*>(
		AddComponent(new LightComponent(this))
	);
}

void PointLight::SetColor(glm::vec3 color) {
	lightComponent->SetColor(color);
}

void PointLight::SetColor(float r, float g, float b) {
	lightComponent->SetColor(r, g, b);
}

void PointLight::SetIntensity(float intensity) {
	lightComponent->SetIntensity(intensity);
}

void PointLight::SetRange(float range) {
	lightComponent->SetRange(range);
}

