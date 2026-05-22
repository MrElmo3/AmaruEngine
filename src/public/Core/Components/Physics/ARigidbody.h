#pragma once
#include <glm/glm.hpp>
#include <vector>

enum class RigidbodyConstraints {
	NONE = 0,

	FREEZE_POSITION_X = 1,
	FREEZE_POSITION_Y = 2,
	FREEZE_POSITION_Z = 4,
	FREEZE_POSITION = 7,

	FREEZE_ROTATION_X = 8,
	FREEZE_ROTATION_Y = 16,
	FREEZE_ROTATION_Z = 32,
	FREEZE_ROTATION = 56,

	FREEZE_ALL = 63
};

enum class Rigidbody2DConstraints {
	NONE = 0,

	FREEZE_POSITION_X = 1,
	FREEZE_POSITION_Y = 2,
	FREEZE_POSITION = 3,

	FREEZE_ROTATION = 4,

	FREEZE_ALL = 7
};

class ARigidbody {
private:
	float mass = 1.f;
	float inverseMass = 1.f;

	float momentOfInertia = 1.f;
	float inverseMomentOfInertia = 1.f;

	float gravityScale = 1.f;

public:
	float GetMass() const { return mass; }
	float GetInverseMass() const { return inverseMass; }

	float GetMomentOfInertia() const { return momentOfInertia; }
	float GetInverseMomentOfInertia() const { return inverseMomentOfInertia; }

	float GetGravityScale() const { return gravityScale; }

	void SetMass(float m) {
		mass = m;
		inverseMass = (m > 0.0f) ? 1.0f / m : 0.0f;
	}

	void SetMomentOfInertia(glm::vec2 size) {
		momentOfInertia = (1.0f / 12.0f) * mass * (size.x * size.x + size.y * size.y);
		inverseMomentOfInertia = 1 / momentOfInertia;
	}

	void SetGravityScale(float g) {
		gravityScale = g;
	}
};