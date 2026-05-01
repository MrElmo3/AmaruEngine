#include "Core/Components/Physics/2D/Rigidbody2DComponent.h"

#include "Core/Components/Physics/2D/ACollider2DComponent.h"
#include "Core/Components/Physics/2D/SquareColliderComponent.h"
#include "Core/Objects/AObject.h"
#include "Core/Physics/PhysicsEngine2D.h"

Rigidbody2DComponent::Rigidbody2DComponent(AObject* parent) : APhysics2DComponent(parent) {
	forceAccumulator = glm::vec2(0);
	velocity = glm::vec2(0);
	aceleration = glm::vec2(0);
	angularVelocity = 0;
	angularAceleration = 0;
	centerOfMass = glm::vec2(0);
}

Rigidbody2DComponent::~Rigidbody2DComponent() {
	forceAccumulator = glm::vec2(0);
	velocity = glm::vec2(0);
	aceleration = glm::vec2(0);
	angularVelocity = 0;
	angularAceleration = 0;
	centerOfMass = glm::vec2(0);
}

void Rigidbody2DComponent::AddForce(glm::vec2 force) {
	forceAccumulator += force;
}
