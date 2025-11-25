#include <Core/Window.h>
#include <Core/Components/Render/Lights/LightComponent.h>
#include <Util/Logger.h>
#include <Core/Render/Shader.h>
#include <string>

LightComponent::LightComponent(AObject* _parent) : IComponent(_parent){
	type = LightType::Point;
	parent->scene->activeLights.insert(this);
	SetRange(1.0f);
}

void LightComponent::Update(double deltaTime) {
	lightPosition = parent->GetWorldPosition();
	lightDirection = parent->Forward();
}

void LightComponent::Use(Shader* shader, int index) {
	if(type == LightType::Directional) {
		shader->SetUInt("_directionalLight.type", (unsigned int)type);
		shader->SetVector3("_directionalLight.direction", lightDirection);
		shader->SetVector3("_directionalLight.color", color);
		shader->SetVector3("_directionalLight.ambient", ambient);
		shader->SetVector3("_directionalLight.diffuse", diffuse);
		shader->SetVector3("_directionalLight.specular", specular);
	}
	else if (type == LightType::Point) {
		std::string lightString = "_activeLights[" + std::to_string(index) + "].";
		shader->SetUInt(lightString + "type", (unsigned int)type);
		shader->SetVector3(lightString + "position", lightPosition);
		shader->SetVector3(lightString + "ambient", ambient);
		shader->SetVector3(lightString + "diffuse", diffuse);
		shader->SetVector3(lightString + "specular", specular);
		shader->SetFloat(lightString + "linear", linearValue);
		shader->SetFloat(lightString + "quadratic", quadraticValue);
	}
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
	linearValue = 4.5/ range;
	quadraticValue = 75.0 / (range * range);
	return this;
}



