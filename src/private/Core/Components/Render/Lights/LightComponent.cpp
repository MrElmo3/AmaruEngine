#include <Core/Window.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>
#include <Core/Render/Shader.h>

LightComponent::LightComponent(AObject* _parent) : IComponent(_parent){
	type = LightType::Point;
	parent->scene->activeLights.insert(this);
}

void LightComponent::Update(double deltaTime) {
	lightPosition = parent->GetWorldPosition();
	lightDirection = parent->Forward();
}

LightComponent* LightComponent::SetType(LightType _type) {
	type = _type;

	if(type == LightType::Directional){
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
	color = _color;
	return this;
}
LightComponent* LightComponent::SetColor(float r, float g, float b) {
	this->color = glm::vec3(r, g, b);
	return this;
}

LightComponent* LightComponent::SetIntensity(float _intensity) {
	intensity = _intensity;
	return this;
}

LightComponent* LightComponent::SetRange(float _range) {
	range = _range;
	return this;
}



