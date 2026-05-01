#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Components/IComponent.h>
#include <Core/Components/Physics/2D/APhysics2DComponent.h>
#include <Core/Components/Physics/ARigidbody.h>

class ACollider2DComponent;

class Rigidbody2DComponent : public APhysics2DComponent, public ARigidbody{

public:
	glm::vec2 velocity;
	float mass = 1;

	Rigidbody2DComponent(AObject* parent);
	~Rigidbody2DComponent() override;

	void Awake() override;
	void End() override;

	void PhysicsUpdate(float fixedDeltaTime) override;

private:
	void DetectCollision(
		ACollider2DComponent* externalCollider,
		ACollider2DComponent* rbCollider,
		float fixedDeltaTime);

public:
	void AddCollider(ACollider2DComponent* collider);
	void RemoveCollider(ACollider2DComponent* collider);
	void ClearColliders();
	std::vector<ACollider2DComponent*> GetColliders();
};
