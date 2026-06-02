#include "Core/Components/Physics/3D/ACollider3DComponent.h"
#include "Core/Objects/AObject.h"

ACollider3DComponent::ACollider3DComponent(AObject* parent, glm::vec3 position) 
	: APhysics3DComponent(parent) {
	this->position = position;
	worldVertexPoints.reserve(4);
	vertexPoints.reserve(4);
	isDirty = true;
}

ACollider3DComponent::ACollider3DComponent(AObject* parent) 
	: ACollider3DComponent(parent, glm::vec3(0)) {
}

ACollider3DComponent::~ACollider3DComponent() {
	position = glm::vec3(0);
	worldPosition = glm::vec3(0);
	APhysics3DComponent::~APhysics3DComponent();
}

void ACollider3DComponent::LateUpdate() {
	if(isDirty) UpdateWorldValues();
	APhysics3DComponent::LateUpdate();
	if (Global::DEBUG) {
		DrawDebugOutline();
	}
}

std::pair<glm::vec3, glm::vec3> ACollider3DComponent::GetAABBContainer() {
	if(isDirty) UpdateWorldValues();
	return ACollider::GetAABBContainer();
}

void ACollider3DComponent::UpdateWorldVertexPoints() {
	worldVertexPoints.clear();
	for(auto vertex : vertexPoints){
		worldVertexPoints.push_back(parent->GetTransformMatrix() * glm::vec4(vertex, 1));
	}
}

glm::vec3 ACollider3DComponent::GetSupportPoint(glm::vec3 direction) {
	return GetSupportPoint(direction);
}

ACollider3DComponent* ACollider3DComponent::SetPosition(glm::vec3 position) {
	this->position = position;
	UpdateWorldPosition();
	UpdateLocalVertexPoints();
	return this;
}

void ACollider3DComponent::UpdateWorldPosition() {
	worldPosition = parent->GetWorldPosition() + position;
	MarkDirty();
}