#include <Core/Objects/Light/SpotLight.h>
#include <Core/Global.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>

SpotLight::SpotLight(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {
	lightComponent = AddComponent<LightComponent>();
	lightComponent->SetType(LightType::Spot)
		->SetSpotRange(25, 35)
		->SetRange(25);
}

SpotLight* SpotLight::SetColor(glm::vec3 color) {
	lightComponent->SetColor(color);
	return this;
}

SpotLight* SpotLight::SetColor(float r, float g, float b) {
	lightComponent->SetColor(r, g, b);
	return this;
}

SpotLight* SpotLight::SetIntensity(float intensity) {
	lightComponent->SetIntensity(intensity);
	return this;
}

SpotLight* SpotLight::SetRange(float range) {
	lightComponent->SetRange(range);
	return this;
}

SpotLight* SpotLight::SetSpotRange(float innerRange, float outerRange){
	lightComponent->SetSpotRange(innerRange, outerRange);
	return this;
}

