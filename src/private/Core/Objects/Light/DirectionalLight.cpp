#include <Core/Global.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Core/Objects/Light/DirectionalLight.h>
#include <Util/Logger.h>

DirectionalLight::DirectionalLight(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {
	lightComponent = AddComponent<LightComponent>();
	lightComponent->SetType(LightType::Directional);
}

DirectionalLight* DirectionalLight::SetColor(glm::vec3 color) {
	lightComponent->SetColor(color);
	return this;
}

DirectionalLight* DirectionalLight::SetColor(float r, float g, float b) {
	lightComponent->SetColor(r, g, b);
	return this;
}

DirectionalLight* DirectionalLight::SetIntensity(float intensity) {
	lightComponent->SetIntensity(intensity);
	return this;
}

