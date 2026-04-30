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
	UpdateVertexPoints();
}

void SquareColliderComponent::UpdateVertexPoints() {
	vertexPoints = {
		glm::vec3(worldCenter.x - worldHalfSize.x, worldCenter.y - worldHalfSize.y, 0), //bottom left
		glm::vec3(worldCenter.x + worldHalfSize.x, worldCenter.y - worldHalfSize.y, 0), //bottom right
		glm::vec3(worldCenter.x + worldHalfSize.x, worldCenter.y + worldHalfSize.y, 0), //top right
		glm::vec3(worldCenter.x - worldHalfSize.x, worldCenter.y + worldHalfSize.y, 0), //top left
	};
}

glm::vec3 SquareColliderComponent::GetSupportPoint(glm::vec2 direction) {
	return GetSupportPoint(glm::vec3(direction, 0.f));
}

glm::vec3 SquareColliderComponent::GetSupportPoint(glm::vec3 direction) {
	float maxDotProduct = 0.f;
	int index = -1;

	for(int i = 0; i < vertexPoints.size(); i++) {
		float currentDotProduct = glm::dot(direction, vertexPoints[i]);

		if(maxDotProduct < currentDotProduct) {
			maxDotProduct = currentDotProduct;
			index = i;
		}
	}
	return vertexPoints[index];
}

void SquareColliderComponent::LateUpdate() {
	if (Global::DEBUG) {
		DrawDebugOutline();
	}
}

SquareColliderComponent* SquareColliderComponent::SetCenter(glm::vec2 center) {
	this->center = center;
	UpdateVertexPoints();
	return this;
}

SquareColliderComponent* SquareColliderComponent::SetHalfSize(glm::vec2 halfSize) {
	this->halfSize = halfSize;
	UpdateVertexPoints();
	return this;
}