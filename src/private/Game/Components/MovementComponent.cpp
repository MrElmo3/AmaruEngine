#include "Game/Components/MovementComponent.h"

#include "Core/Global.h"
#include "Core/Components/Physics/3D/Rigidbody3DComponent.h"
#include "Core/Objects/AObject.h"
#include "Util/Logger.h"

MovementComponent::MovementComponent(AObject* parent) : IComponent(parent) {
	direction = glm::vec2(0);
	rotation = glm::vec2(0);
	
	speed = 0;
	rotationSpeed = 0;
}

MovementComponent::~MovementComponent() {
}

void MovementComponent::Start() {
	rb = parent->GetComponent<Rigidbody3DComponent>();
	if (rb == nullptr) {
		Logger::Error("MovementComponent::Start -> RigidbodyComponent not found");
	}
}

void MovementComponent::Update(double deltaTime) {
	glm::vec3 normalizeDirection = parent->Forward() * direction.y + parent->Right() * direction.x;
	normalizeDirection = normalizeDirection != glm::vec3(0.0f)? glm::normalize(normalizeDirection) : normalizeDirection;
	glm::vec3 velocity = normalizeDirection * speed;
	rb->velocity = velocity;

	//For now the rigidbody will not manage the rotation
	parent->RotateEuler(glm::vec3(-rotation.y, rotation.x, 0) * rotationSpeed * Global::FIXED_DELTA_TIME);
}

void MovementComponent::FixedUpdate() {
	
}


MovementComponent* MovementComponent::SetSpeed(float speed) {
	this->speed = speed;
	return this;
}

MovementComponent* MovementComponent::SetRotationSpeed(float rotationSpeed) {
	this->rotationSpeed = rotationSpeed;
	return this;
}

MovementComponent* MovementComponent::SetDirection(glm::vec2 direction) {
	this->direction = direction;
	return this;
}

MovementComponent* MovementComponent::SetRotation(glm::vec2 rotation) {
	this->rotation = rotation;
	return this;
}