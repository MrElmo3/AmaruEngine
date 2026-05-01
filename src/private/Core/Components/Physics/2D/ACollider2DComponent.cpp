#include <Core/Components/Physics/2D/ACollider2DComponent.h>
#include <Core/Objects/AObject.h>

ACollider2DComponent::ACollider2DComponent(AObject* parent, glm::vec2 position) 
	: APhysics2DComponent(parent) {
	this->position = position;
}

ACollider2DComponent::ACollider2DComponent(AObject* parent) 
	: ACollider2DComponent(parent, glm::vec2(0)) {
}

void ACollider2DComponent::FixedUpdate() {
	UpdateWorldPosition();
	UpdateVertexPoints();
	APhysics2DComponent::FixedUpdate();
}

ACollider2DComponent* ACollider2DComponent::SetPosition(glm::vec2 position) {
	this->position = position;
	UpdateWorldPosition();
	UpdateVertexPoints();
	return this;
}

void ACollider2DComponent::UpdateWorldPosition() {
	worldPosition = parent->GetWorldPosition() + glm::vec3(position, 0) ;
}