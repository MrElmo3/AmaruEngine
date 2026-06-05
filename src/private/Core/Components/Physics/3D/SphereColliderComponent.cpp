#include <Core/Components/Physics/3D/SphereColliderComponent.h>

SphereColliderComponent::SphereColliderComponent(AObject* parent, glm::vec3 position, float radius)
	: ACollider3DComponent(parent, position) {
	SetRadius(radius);
}

SphereColliderComponent::SphereColliderComponent(AObject* parent, glm::vec3 position)
	:  SphereColliderComponent(parent, position, 0.5f) {}

SphereColliderComponent::SphereColliderComponent(AObject* parent)
	: SphereColliderComponent(parent, glm::vec3(0)) {
}

SphereColliderComponent::~SphereColliderComponent() {
	SetRadius(0.f);
	ACollider3DComponent::~ACollider3DComponent();
}

void SphereColliderComponent::UpdateLocalVertexPoints() {
	vertexPoints.clear();
	vertexPoints.reserve(RENDER_VERTEX_NUMBER * 3);

	float angleStep = 360.0f / RENDER_VERTEX_NUMBER;

	for (int i = 0; i < RENDER_VERTEX_NUMBER; i++) {
		float angleInRadians = glm::radians(angleStep * i);

		float cosA = glm::cos(angleInRadians);
		float sinA = glm::sin(angleInRadians);

		vertexPoints.push_back(glm::vec3(radius * cosA, radius * sinA, 0.0f));
		vertexPoints.push_back(glm::vec3(radius * cosA, 0.0f, radius * sinA));
		vertexPoints.push_back(glm::vec3(0.0f, radius * cosA, radius * sinA));
	}
}

void SphereColliderComponent::UpdateRenderIndexes() {
	renderIndexes.clear();
	for (int i = 0; i < 3; i++) {
		for(int j = 0; j < RENDER_VERTEX_NUMBER; j++) {
			renderIndexes.push_back(i + j * 3);
			renderIndexes.push_back(j+1 == RENDER_VERTEX_NUMBER ? i : i + (j + 1) * 3);
		}
	}
}

glm::vec3 SphereColliderComponent::GetSupportPoint(glm::vec3 direction) {
	if(glm::length(direction) < 0.0001f) return ACollider::GetSupportPoint(direction);

	return center + glm::normalize(direction) * radius;
}

SphereColliderComponent* SphereColliderComponent::SetRadius(float radius) {
	this->radius = radius;
	UpdateLocalVertexPoints();
	return this;
}