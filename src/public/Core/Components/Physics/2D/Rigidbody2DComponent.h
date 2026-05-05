#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Components/IComponent.h>
#include <Core/Components/Physics/2D/APhysics2DComponent.h>
#include <Core/Components/Physics/ARigidbody.h>

class ACollider2DComponent;

class Rigidbody2DComponent : public APhysics2DComponent, public ARigidbody{

public:
	glm::vec2 forceAccumulator;

	glm::vec2 velocity;
	glm::vec2 aceleration;

	float angularVelocity;
	float angularAceleration;

	glm::vec2 centerOfMass;
	
	Rigidbody2DConstraints RigidbodyConstraints = Rigidbody2DConstraints::NONE;

	Rigidbody2DComponent(AObject* parent);
	virtual ~Rigidbody2DComponent() override;

private:
	bool ParentHasRigidbody();

public:
	void AddForce(glm::vec2 force);
};
