#include "Core/Components/Physics/2D/SquareColliderComponent.h"

#include "Core/Global.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Color.h"
#include "Core/Render/Render.h"

SquareColliderComponent::SquareColliderComponent(AObject* parent)
	: ACollider2DComponent(parent) {
	center = glm::vec2(0.0);
	halfSize = glm::vec2(0.5);
}

SquareColliderComponent::SquareColliderComponent(
	AObject* parent,
	glm::vec2 center,
	glm::vec2 halfSize)
	: SquareColliderComponent(parent) {

	this->center = center;
	this->halfSize = halfSize;
}

SquareColliderComponent::~SquareColliderComponent() {
}

void SquareColliderComponent::FixedUpdate() {
	worldCenter = parent->GetWorldPosition() + glm::vec3(center, 0) ;

	worldHalfSize = glm::vec2(
		halfSize.x * parent->GetWorldScale().x,
		halfSize.y * parent->GetWorldScale().y
	);
}

void SquareColliderComponent::LateUpdate() {
	if (Global::DEBUG) {
		Render::GetInstance().DrawQuadLine(
			worldCenter,
			worldHalfSize * 2.001f,
			Color::GREEN);
	}
}

SquareColliderComponent* SquareColliderComponent::SetCenter(glm::vec2 center) {
	this->center = center;
	return this;
}

SquareColliderComponent* SquareColliderComponent::SetHalfSize(glm::vec2 halfSize) {
	this->halfSize = halfSize;
	return this;
}