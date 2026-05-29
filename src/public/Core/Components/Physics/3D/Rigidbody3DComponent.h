#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Components/IComponent.h>
#include <Core/Components/Physics/2D/APhysics2DComponent.h>
#include <Core/Components/Physics/ARigidbody.h>

class ACollider3DComponent;

class Rigidbody3DComponent : public APhysics2DComponent, public ARigidbody{

public:
	glm::vec3 forceAccumulator;

	glm::vec3 velocity;
	glm::vec3 acceleration;

	//TODO: add angular velocity and acceleration
	// float angularVelocity;
	// float angularAcceleration;

	glm::vec3 centerOfMass;
	
	RigidbodyConstraints RigidbodyConstraints = RigidbodyConstraints::NONE;

	Rigidbody3DComponent(AObject* parent);
	virtual ~Rigidbody3DComponent() override;

private:
	bool ParentHasRigidbody();

public:
	void AddForce(glm::vec3 force);
};
