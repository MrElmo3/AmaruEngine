#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Util/Logger.h>

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

public:
	float gravityScale = 1.f;

	float GetMass() const { return mass; }
	float GetInverseMass() const { return inverseMass;}

	void SetMass(float mass) {
		if(mass <= 0) {
			Logger::Warning("The mass cannot be less or equal to 0.");
			return;
		}
		this->mass = mass;
		inverseMass = 1.f / mass;
	}
};