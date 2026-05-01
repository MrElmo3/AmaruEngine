#pragma once
#include <vector>
#include <glm/glm.hpp>

enum RigidbodyConstraints {
	NONE = 0,

	FREEZE_POSITION_X = 2,
	FREEZE_POSITION_Y = 4,
	FREEZE_POSITION_Z = 8,

	FREEZE_POSITION_X = 16,
	FREEZE_POSITION_Y = 32,
	FREEZE_POSITION_Z = 64,

	FREEZE_POSITION = 14,
	FREEZE_ROTATION = 112,
	FREEZE_ALL = 126
};

class ACollider;
class ARigidbody {

protected:
	std::vector<ACollider*> colliders;

public:
	glm::vec3 velocity;
	glm::vec3 aceleration;
	
	glm::vec3 angularVelocity;
	glm::vec3 angularAceleration;

	float mass = 1.f;
	float gravityScale = 1.f;

	RigidbodyConstraints constraints = RigidbodyConstraints::NONE;

protected:
	virtual void PhysicsUpdate(float fixedDeltaTime);
	void SetForce(glm::vec3 force);
};