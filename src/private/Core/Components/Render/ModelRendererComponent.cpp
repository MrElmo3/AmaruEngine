#include <Core/Components/Render/ModelRendererComponent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Render.h>
#include <Core/Render/Model.h>

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
	if(model == nullptr) return;
	if (!enableRender) return;
	model->Draw();
}

ModelRendererComponent* ModelRendererComponent::SetModel(Model* model){
	this->model = model;
	return this;
}