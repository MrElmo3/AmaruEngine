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
	else {
		std::string lightString = "_activeLights[" + std::to_string(index) + "].";
		shader->SetUInt(lightString + "type", (unsigned int)type);
		shader->SetVector3(lightString + "position", lightPosition);
		shader->SetVector3(lightString + "ambient", ambient);
		shader->SetVector3(lightString + "diffuse", diffuse);
		shader->SetVector3(lightString + "specular", specular);
		shader->SetFloat(lightString + "linear", linearValue);
		shader->SetFloat(lightString + "quadratic", quadraticValue);

		if( type == LightType::Spot) {
			shader->SetVector3(lightString + "direction", lightDirection);
			shader->SetFloat(lightString + "innerRange", innerRange);
			shader->SetFloat(lightString + "outerRange", outerRange);
		}
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
	if(type == LightType::Directional) {
		Logger::Warning(
			(std::string)"Trying to set range on a light of type Directional. " + 
			"Light name: " + parent->name);
		return this;
	}
	range = _range;
	linearValue = 4.5/ range;
	quadraticValue = 75.0 / (range * range);
	return this;
}

LightComponent* LightComponent::SetSpotRange(float _inner, float _outer) {
	
	if(type != LightType::Spot) {
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

	innerRange = glm::cos(glm::radians(_inner));
	outerRange = glm::cos(glm::radians(_outer));
	return this;
}



