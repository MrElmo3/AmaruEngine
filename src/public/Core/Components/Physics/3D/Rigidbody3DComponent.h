#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Components/IComponent.h>
#include <Core/Components/Physics/3D/APhysics3DComponent.h>
#include <Core/Components/Physics/ARigidbody.h>

class ACollider3DComponent;

class Rigidbody3DComponent : public APhysics3DComponent, public ARigidbody{

public:
	glm::vec3 forceAccumulator;

	glm::vec3 velocity;
	glm::vec3 acceleration;

	//TODO: add angular velocity and acceleration
	// float angularVelocity;
	// float angularAcceleration;

	glm::vec3 centerOfMass;

	/// @brief Bounciness of the body [0, 1]. 0 = no bounce, 1 = perfectly elastic.
	float restitution = 0.3f;

	/// @brief Coulomb friction coefficient used when sliding over other colliders.
	float friction = 0.4f;
	
	RigidbodyConstraints constraints = RigidbodyConstraints::NONE;

	Rigidbody3DComponent(AObject* parent);
	virtual ~Rigidbody3DComponent() override;

private:
	bool ParentHasRigidbody();

public:
	void AddForce(glm::vec3 force);
};
