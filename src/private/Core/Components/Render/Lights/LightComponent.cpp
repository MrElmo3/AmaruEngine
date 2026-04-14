#include <Core/Window.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>
#include <Core/Render/Shader.h>
#include <string>

LightComponent::LightComponent(AObject* _parent) : IComponent(_parent){
	data.type = LightType::POINT;
	parent->scene->activeLights.insert(this);
	SetRange(1.0f);
}

void LightComponent::LateUpdate() {
	data.position = parent->GetWorldPosition();
	data.direction = parent->Forward();
}

LightData LightComponent::GetLightData() {
	return data;
}

LightComponent* LightComponent::SetType(LightType _type) {
	data.type = _type;

	if(data.type == LightType::DIRECTIONAL){
		if (parent->scene->directionalLight) {
			Logger::Error("The actual scene already have a LightSource");
			return nullptr;
		}
		parent->scene->activeLights.erase(this);
		parent->scene->directionalLight = this;
	}
	return this;
}

LightComponent* LightComponent::SetColor(glm::vec3 _color) {
	data.color = _color;
	return this;
}
LightComponent* LightComponent::SetColor(float r, float g, float b) {
	data.color = glm::vec3(r, g, b);
	return this;
}

// LightComponent* LightComponent::SetIntensity(float _intensity) {
	// data.intensity = _intensity;
// 	return this;
// }

LightComponent* LightComponent::SetRange(float _range) {
	if(data.type == LightType::DIRECTIONAL) {
		Logger::Warning(
			(std::string)"Trying to set range on a light of type Directional. " + 
			"Light name: " + parent->name);
		return this;
	}
	range = _range;
	data.linearAtenuation = 16/ range;
	data.quadraticAtenuation = 75.0 / (range * range);
	return this;
}

LightComponent* LightComponent::SetSpotRange(float _inner, float _outer) {
	
	if(data.type != LightType::SPOT) {
		Logger::Warning(
			(std::string)"Trying to set SpotRange on a light of type different than Spot. " + 
			"Light name: " + parent->name);
		return this;
	}
	
	if(_inner < 0) {
		Logger::Warning(
			(std::string)"Inner range is set bellow 0. " + 
			"Light name: " + parent->name);
		return this;
	}

	if(_outer > 180) {
		Logger::Warning(
			(std::string)"Outer range is set upon 180. " + 
			"Light name: " + parent->name);
		return this;
	}

	if(_outer < _inner) {
		Logger::Warning(
			(std::string)"Outer range is set bellow inner range. " + 
			"Light name: " + parent->name);
		return this;
	}

	data.innerRange = glm::cos(glm::radians(_inner));
	data.outerRange = glm::cos(glm::radians(_outer));
	return this;
}



