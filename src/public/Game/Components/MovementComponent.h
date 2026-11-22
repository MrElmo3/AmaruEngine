#pragma once
#include <glm/glm.hpp>

#include "Core/Components/IComponent.h"


class Rigidbody3DComponent;

class MovementComponent : public IComponent {
private:
	glm::vec2 direction;

	float speed;
	// Degrees rotated per pixel of mouse movement.
	float lookSensitivity;

	Rigidbody3DComponent* rb;

	static constexpr float MAX_PITCH = 89.0f;

public:
	MovementComponent(AObject* parent);
	~MovementComponent() override;

	void Start() override;
	void Update(double deltaTime) override;
	void FixedUpdate() override;

	MovementComponent* SetSpeed(float speed);
	MovementComponent* SetLookSensitivity(float lookSensitivity);
	MovementComponent* SetDirection(glm::vec2 direction);
	// Rotates the object by a mouse delta in pixels (x = right, y = up).
	void Look(glm::vec2 mouseDelta);

	glm::vec2 GetDirection()	const { return direction; }
	double GetSpeed()			const { return speed; }
	double GetLookSensitivity()	const { return lookSensitivity; }
};
