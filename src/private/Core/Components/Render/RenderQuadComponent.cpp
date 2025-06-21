#include "Core/Components/Render/RenderQuadComponent.h"

#include <Core/Materials/LitMaterial.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Render/Shader.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Render.h"

RenderQuadComponent::RenderQuadComponent(AObject* parent) : IComponent(parent){
	material = new LitMaterial();
}

RenderQuadComponent::~RenderQuadComponent() {
	delete this->material;
}

void RenderQuadComponent::LateUpdate() {
	Draw();
}

void RenderQuadComponent::Draw() const {
	if (!enableRender) return;

	glm::mat4 model = Render::GetInstance().GetModelMatrix(parent);
	Render::GetInstance().DrawQuad(&model, material);
}
