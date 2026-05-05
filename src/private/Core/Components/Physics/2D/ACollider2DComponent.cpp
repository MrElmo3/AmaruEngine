#include <Core/Components/Physics/2D/ACollider2DComponent.h>
#include <Core/Objects/AObject.h>

ACollider2DComponent::ACollider2DComponent(AObject* parent, glm::vec2 position) 
	: APhysics2DComponent(parent) {
	this->position = position;
	worldVertexPoints.reserve(4);
	vertexPoints.reserve(4);
	isDirty = true;
}

ACollider2DComponent::ACollider2DComponent(AObject* parent) 
	: ACollider2DComponent(parent, glm::vec2(0)) {
}

ACollider2DComponent::~ACollider2DComponent() {
	position = glm::vec2(0);
	worldPosition = glm::vec2(0);
	APhysics2DComponent::~APhysics2DComponent();
}

void ACollider2DComponent::LateUpdate() {
	if(isDirty) UpdateWorldVertexPoints();
	APhysics2DComponent::LateUpdate();
}

ACollider2DComponent* ACollider2DComponent::SetPosition(glm::vec2 position) {
	this->position = position;
	UpdateWorldPosition();
	UpdateLocalVertexPoints();
	return this;
}

void ACollider2DComponent::UpdateWorldPosition() {
	worldPosition = parent->GetWorldPosition() + glm::vec3(position, 0);
	MarkDirty();
}

void ACollider2DComponent::UpdateWorldVertexPoints() {
	worldVertexPoints.clear();
	for(auto vertex : vertexPoints){
		worldVertexPoints.push_back(parent->GetTransformMatrix() * glm::vec4(vertex, 1));
	}
}