#include <Core/Components/Render/ModelRendererComponent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Render.h>
#include <Core/Render/Model.h>

ModelRendererComponent::ModelRendererComponent(AObject* parent) : IComponent(parent){
	material = new LitMaterial();
	model = nullptr;
}

ModelRendererComponent::ModelRendererComponent(AObject* parent, std::string modelPath) : ModelRendererComponent(parent){
	model = new Model(modelPath);
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