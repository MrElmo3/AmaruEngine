#include <Core/Window.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>

LightComponent::LightComponent(AObject* _parent) : IComponent(_parent){
	type = LightType::Point;

	range = 1.0f;
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	intensity = 1.0f;

	if (parent->scene->lightSource) {
		Logger::Error("The actual scene already have a LightSource");
		return;
	}
	parent->scene->lightSource = this;
}

void LightComponent::Update(double deltaTime) {
	lightPosition = parent->GetWorldPosition();
}


void LightComponent::SetColor(glm::vec3 color) {
	this->color = color;
}
void LightComponent::SetColor(float r, float g, float b) {
	this->color = glm::vec3(r, g, b);
}

void LightComponent::SetIntensity(float intensity) {
	this->intensity = intensity;
}

void LightComponent::SetRange(float range) {
	this->range = range;
}



