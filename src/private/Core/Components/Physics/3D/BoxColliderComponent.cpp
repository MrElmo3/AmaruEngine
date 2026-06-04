#include "Core/Components/Physics/3D/BoxColliderComponent.h"

BoxColliderComponent::BoxColliderComponent(
	AObject* parent,
	glm::vec3 position,
	glm::vec3 halfSize)
	: ACollider3DComponent(parent, position) {
	SetHalfSize(halfSize);
}

BoxColliderComponent::BoxColliderComponent(AObject* parent, glm::vec3 position)
	:  BoxColliderComponent(parent, position, glm::vec3(0.5f)) {}

BoxColliderComponent::BoxColliderComponent(AObject* parent)
	: BoxColliderComponent(parent, glm::vec3(0)) {
}

BoxColliderComponent::~BoxColliderComponent() {
	SetHalfSize(glm::vec3(0));
	ACollider3DComponent::~ACollider3DComponent();
}

void BoxColliderComponent::UpdateLocalVertexPoints() {
	vertexPoints = {
		glm::vec3(position.x - halfSize.x, position.y - halfSize.y, position.z + halfSize.z), //bottom left
		glm::vec3(position.x + halfSize.x, position.y - halfSize.y, position.z + halfSize.z), //bottom right
		glm::vec3(position.x + halfSize.x, position.y + halfSize.y, position.z + halfSize.z), //top right
		glm::vec3(position.x - halfSize.x, position.y + halfSize.y, position.z + halfSize.z), //top left

		glm::vec3(position.x - halfSize.x, position.y - halfSize.y, position.z - halfSize.z), //bottom left
		glm::vec3(position.x + halfSize.x, position.y - halfSize.y, position.z - halfSize.z), //bottom right
		glm::vec3(position.x + halfSize.x, position.y + halfSize.y, position.z - halfSize.z), //top right
		glm::vec3(position.x - halfSize.x, position.y + halfSize.y, position.z - halfSize.z), //top left
	};
}

glm::vec3 BoxColliderComponent::GetSupportPoint(glm::vec3 direction) {
	if(glm::length(direction) < 0.0001f) return ACollider::GetSupportPoint(direction);
	
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

BoxColliderComponent* BoxColliderComponent::SetHalfSize(glm::vec3 halfSize) {
	this->halfSize = halfSize;
	UpdateLocalVertexPoints();
	return this;
}