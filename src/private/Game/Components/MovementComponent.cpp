#include "Game/Components/MovementComponent.h"

#include <glm/gtc/quaternion.hpp>

#include "Core/Global.h"
#include "Core/Components/Physics/3D/Rigidbody3DComponent.h"
#include "Core/Objects/AObject.h"
#include "Util/Logger.h"

MovementComponent::MovementComponent(AObject* parent) : IComponent(parent) {
	direction = glm::vec2(0);

	speed = 0;
	lookSensitivity = 0.1f;
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
}

void MovementComponent::FixedUpdate() {

}


MovementComponent* MovementComponent::SetSpeed(float speed) {
	this->speed = speed;
	return this;
}

MovementComponent* MovementComponent::SetLookSensitivity(float lookSensitivity) {
	this->lookSensitivity = lookSensitivity;
	return this;
}

MovementComponent* MovementComponent::SetDirection(glm::vec2 direction) {
	this->direction = direction;
	return this;
}

void MovementComponent::Look(glm::vec2 mouseDelta) {
	// The mouse delta is already the distance moved this frame, so no deltaTime here.
	float yawDegrees = mouseDelta.x * lookSensitivity;
	float pitchDegrees = mouseDelta.y * lookSensitivity;

	// Clamp the pitch so the camera can never flip over the top or bottom.
	float currentPitch = glm::degrees(asin(glm::clamp(parent->Forward().y, -1.0f, 1.0f)));
	float targetPitch = glm::clamp(currentPitch + pitchDegrees, -MAX_PITCH, MAX_PITCH);
	pitchDegrees = targetPitch - currentPitch;

	// Yaw around the world up axis and pitch around the local right axis,
	// so combining both never rolls the camera.
	glm::quat yaw = glm::angleAxis(glm::radians(-yawDegrees), Global::UP);
	glm::quat pitch = glm::angleAxis(glm::radians(pitchDegrees), Global::RIGHT);
	parent->SetRotation(yaw * parent->GetRotation() * pitch);
}
