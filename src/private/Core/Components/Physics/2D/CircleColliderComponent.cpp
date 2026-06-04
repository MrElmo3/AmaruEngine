#include "Core/Components/Physics/2D/CircleColliderComponent.h"

#include "Core/Global.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Color.h"
#include "Core/Render/Render.h"

CircleColliderComponent::CircleColliderComponent(AObject* parent, glm::vec2 position, float radius)
	: ACollider2DComponent(parent, position) {
	SetRadius(radius);
}

CircleColliderComponent::CircleColliderComponent(AObject* parent, glm::vec2 position)
	:  CircleColliderComponent(parent, position, 0.5f) {}

CircleColliderComponent::CircleColliderComponent(AObject* parent)
	: CircleColliderComponent(parent, glm::vec2(0)) {
}

CircleColliderComponent::~CircleColliderComponent() {
	SetRadius(0.f);
	ACollider2DComponent::~ACollider2DComponent();
}

void CircleColliderComponent::UpdateLocalVertexPoints() {
	vertexPoints.clear();
	vertexPoints.reserve(RENDER_VERTEX_NUMBER);
	
	float angleStep = 360.0f / RENDER_VERTEX_NUMBER;

	for (int i = 0; i < RENDER_VERTEX_NUMBER; i++) {
		float angleInRadians = glm::radians(angleStep * i);

		float x = radius * glm::cos(angleInRadians);
		float y = radius * glm::sin(angleInRadians);
		vertexPoints.push_back(glm::vec3(x, y, 0.0f));
	}
}

glm::vec3 CircleColliderComponent::GetSupportPoint(glm::vec3 direction) {
	if(glm::length(direction) < 0.0001f) return ACollider::GetSupportPoint(direction);
	if(glm::length(glm::vec2(direction.x, direction.y)) < 0.0001f) return glm::vec3(NAN);

	return center + glm::normalize(direction) * radius;
}

CircleColliderComponent* CircleColliderComponent::SetRadius(float radius) {
	this->radius = radius;
	UpdateLocalVertexPoints();
	return this;
}