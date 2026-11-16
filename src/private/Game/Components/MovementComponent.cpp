#include "Game/Components/MovementComponent.h"

#include <glm/gtc/quaternion.hpp>

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
	// Walk on the ground plane: looking up/down must not lift the player over the walls.
	glm::vec3 forward = parent->Forward();
	glm::vec3 right = parent->Right();
	forward.y = 0.0f;
	right.y = 0.0f;
	glm::vec3 normalizeDirection = forward * direction.y + right * direction.x;
	normalizeDirection = normalizeDirection != glm::vec3(0.0f)? glm::normalize(normalizeDirection) : normalizeDirection;
	glm::vec3 velocity = normalizeDirection * speed;
	rb->velocity = velocity;

	//For now the rigidbody will not manage the rotation
	if (rotation != glm::vec2(0)) {
		// rotationSpeed is in degrees per second, whatever the frame rate is.
		float step = glm::radians(rotationSpeed * (float)deltaTime);
		// Yaw around the world up axis and pitch around the local right axis,
		// so combining both never rolls the camera.
		glm::quat yaw = glm::angleAxis(-rotation.x * step, Global::UP);
		glm::quat pitch = glm::angleAxis(rotation.y * step, Global::RIGHT);
		parent->SetRotation(yaw * parent->GetRotation() * pitch);
	}
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