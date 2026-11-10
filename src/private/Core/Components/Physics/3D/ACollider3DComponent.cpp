#include "Core/Components/Physics/3D/ACollider3DComponent.h"
#include "Core/Objects/AObject.h"

ACollider3DComponent::ACollider3DComponent(AObject* parent, glm::vec3 position)
	: APhysics3DComponent(parent) {
	this->position = position;
	worldVertexPoints.reserve(8);
	vertexPoints.reserve(8);
	isDirty = true;
}

ACollider3DComponent::ACollider3DComponent(AObject* parent)
	: ACollider3DComponent(parent, glm::vec3(0)) {
}

ACollider3DComponent::~ACollider3DComponent() {}

void ACollider3DComponent::LateUpdate() {
	EnsureWorldValues();
	APhysics3DComponent::LateUpdate();
	if (Global::DEBUG) {
		DrawDebugOutline();
	}
}

void ACollider3DComponent::MarkDirty() {
	APhysics3DComponent::MarkDirty();
	worldValuesDirty = true;
}

void ACollider3DComponent::EnsureWorldValues() {
	if (!worldValuesDirty) return;
	worldValuesDirty = false;
	UpdateWorldValues();
}

std::pair<glm::vec3, glm::vec3> ACollider3DComponent::GetAABBContainer() {
	EnsureWorldValues();
	return ACollider::GetAABBContainer();
}

void ACollider3DComponent::UpdateWorldVertexPoints() {
	glm::mat4 transform = parent->GetTransformMatrix();

	worldVertexPoints.clear();
	for(auto vertex : vertexPoints){
		worldVertexPoints.push_back(glm::vec3(transform * glm::vec4(vertex, 1)));
	}
	worldPosition = glm::vec3(transform * glm::vec4(position, 1));
}

ACollider3DComponent* ACollider3DComponent::SetPosition(glm::vec3 position) {
	this->position = position;
	UpdateLocalVertexPoints();
	MarkDirty();
	return this;
}
