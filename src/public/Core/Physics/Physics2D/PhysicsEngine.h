#pragma once
#include <Core/Physics/APhysicsEngine.h>
#include <glm/glm.hpp>
#include <vector>

class AObject;
class ACollider2DComponent;
class Rigidbody2DComponent;

namespace Physics2D {

class PhysicObject;
class Quadtree;

/**
 * This class encasulates all the physics calculations for 2D objects.
 */
class PhysicsEngine : public APhysicsEngine {

private:
	struct CollisionManifold2D{
		glm::vec3 PenetrationVector;
		float Depth;
	};

	std::vector<PhysicObject*> physicObjects;

	Quadtree* quadtree;
	glm::vec2 worldMin = { -500, -500 };
	glm::vec2 worldMax = { 500, 500 };

public:
	PhysicsEngine();

protected:
	void RegisterObject(AObject* object) override;

	void MoveObjects() override;
	void MoveRigidbody(Rigidbody2DComponent* rigidbody);
	void UpdateAceleration(Rigidbody2DComponent* rigidbody);
	void UpdateVelocity(Rigidbody2DComponent* rigidbody);
	void UpdatePosition(Rigidbody2DComponent* rigidbody);

	void UpdateTree() override;
	void UpdateContainers();

	void CheckCollisions() override;
	bool CheckCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB);
	void ResolveCollision2D(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold);

private:
	void Resolve2RBCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold);
	void Resolve1RBCollision(PhysicObject* physicObject, CollisionManifold manifold);
};
}
