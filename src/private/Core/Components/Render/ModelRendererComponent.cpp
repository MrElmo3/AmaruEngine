#include <Core/Components/Render/ModelRendererComponent.h>

#include <Core/Materials/LitMaterial.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Objects/AObject.h"
#include "Core/Render/Render.h"

ModelRendererComponent::ModelRendererComponent(AObject* parent) : IComponent(parent){
	material = new LitMaterial();
}

ModelRendererComponent::~ModelRendererComponent() {
	delete this->material;
}

void ModelRendererComponent::LateUpdate() {
	Draw();
}

void ModelRendererComponent::Draw() {
	if (!enableRender) return;
	
}