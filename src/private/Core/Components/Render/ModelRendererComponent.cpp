#include <Core/Components/Render/ModelRendererComponent.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Render.h>
#include <Core/Render/Model.h>
#include <Core/Render/Shader.h>
#include <string>

ModelRendererComponent::ModelRendererComponent(AObject* parent) : IComponent(parent){
	material = new LitMaterial();
	model = nullptr;
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
	
	material->Use();
	
	glm::mat4 modelMatrix = Render::GetInstance().GetModelMatrix(parent);
	model->Draw();
}

ModelRendererComponent* ModelRendererComponent::SetModel(Model* model){
	this->model = model;
	return this;
}

ModelRendererComponent* ModelRendererComponent::SetModel(std::string modelPath){
	this->model = new Model(modelPath);
	return this;
}