#include "Core/Components/Physics/2D/SquareColliderComponent.h"

#include "Core/Global.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Color.h"
#include "Core/Render/Render.h"

SquareColliderComponent::SquareColliderComponent(
	AObject* parent,
	glm::vec2 position,
	glm::vec2 halfSize)
	: ACollider2DComponent(parent, position) {
	SetHalfSize(halfSize);
}

SquareColliderComponent::SquareColliderComponent(AObject* parent, glm::vec2 position)
	:  SquareColliderComponent(parent, position, glm::vec2(0.5f)) {}

SquareColliderComponent::SquareColliderComponent(AObject* parent)
	: SquareColliderComponent(parent, glm::vec2(0)) {
}

SquareColliderComponent::~SquareColliderComponent() {
	SetHalfSize(glm::vec2(0));
	ACollider2DComponent::~ACollider2DComponent();
}

void SquareColliderComponent::UpdateLocalVertexPoints() {
	vertexPoints = {
		glm::vec3(position.x - halfSize.x, position.y - halfSize.y, 0), //bottom left
		glm::vec3(position.x + halfSize.x, position.y - halfSize.y, 0), //bottom right
		glm::vec3(position.x + halfSize.x, position.y + halfSize.y, 0), //top right
		glm::vec3(position.x - halfSize.x, position.y + halfSize.y, 0), //top left
	};
}

glm::vec3 SquareColliderComponent::GetSupportPoint(glm::vec3 direction) {
	if(glm::length(direction) < 0.0001f) ACollider::GetSupportPoint(direction);
	if(glm::length(glm::vec2(direction.x, direction.y)) < 0.0001f) return glm::vec3(NAN);
	
	float maxDotProduct = -FLT_MAX;
	int index = -1;

	for(int i = 0; i < worldVertexPoints.size(); i++) {
		float currentDotProduct = glm::dot(direction, worldVertexPoints[i]);

		if(maxDotProduct < currentDotProduct) {
			maxDotProduct = currentDotProduct;
			index = i;
		}
	}
	return worldVertexPoints[index];
}

SquareColliderComponent* SquareColliderComponent::SetHalfSize(glm::vec2 halfSize) {
	this->halfSize = halfSize;
	UpdateLocalVertexPoints();
	return this;
}