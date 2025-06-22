#include "Core/Components/Render/RenderCubeComponent.h"

#include <Core/Materials/LitMaterial.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Objects/AObject.h"
#include "Core/Render/Render.h"

RenderCubeComponent::RenderCubeComponent(AObject* parent) : IComponent(parent){
	material = new LitMaterial();
}

RenderCubeComponent::~RenderCubeComponent() {
	delete this->material;
}

void RenderCubeComponent::LateUpdate() {
	Draw();
}

void RenderCubeComponent::Draw() {
	if (!enableRender) return;
	
	glm::mat4 model = Render::GetInstance().GetModelMatrix(parent);
	Render::GetInstance().DrawCube(&model, material, uv);
}