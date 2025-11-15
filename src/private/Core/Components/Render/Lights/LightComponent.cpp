#include <Core/Window.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>
#include <Core/Render/Shader.h>

LightComponent::LightComponent(AObject* _parent) : IComponent(_parent){
	type = LightType::Point;

	if (parent->scene->lightSource) {
		Logger::Error("The actual scene already have a LightSource");
		return;
	}
	parent->scene->lightSource = this;
}

void LightComponent::Update(double deltaTime) {
	lightPosition = parent->GetWorldPosition();
}

void LightComponent::Use(Shader* shader) {
	shader->SetVector3("_light.position", lightPosition);
	shader->SetVector3("_light.color", color);
	shader->SetVector3("_light.ambient", ambient);
	shader->SetVector3("_light.diffuse", diffuse);
	shader->SetVector3("_light.specular", specular);
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



