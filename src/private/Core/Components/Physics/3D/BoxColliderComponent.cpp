#include "Core/Components/Physics/3D/BoxColliderComponent.h"

#include "Core/Global.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Render.h"


BoxColliderComponent::BoxColliderComponent(AObject* parent)
	: ACollider3DComponent(parent) {
	center = glm::vec3(0.0);
	halfSize = glm::vec3(0.5);
}

BoxColliderComponent::BoxColliderComponent(
	AObject* parent,
	glm::vec3 center,
	glm::vec3 halfSize)
	: BoxColliderComponent(parent) {

	this->center = center;
	this->halfSize = halfSize;
}

BoxColliderComponent::~BoxColliderComponent() {
}

void BoxColliderComponent::FixedUpdate() {
	worldCenter = parent->GetWorldPosition() + center;

	worldHalfSize = glm::vec3(
		halfSize.x * parent->GetWorldScale().x,
		halfSize.y * parent->GetWorldScale().y,
		halfSize.z * parent->GetWorldScale().z
	);
}

void BoxColliderComponent::LateUpdate() {
	if (Global::DEBUG) {
		// Render::GetInstance().DrawCubeLine(
		// 	worldCenter,
		// 	worldHalfSize * 2.0f,
		// 	Color::GREEN);
	}
}

BoxColliderComponent* BoxColliderComponent::SetCenter(glm::vec3 center){
	this->center = center;
	return this;
}

BoxColliderComponent* BoxColliderComponent::SetHalfSize(glm::vec3 halfSize){
	this->halfSize = halfSize;
	return this;
}
