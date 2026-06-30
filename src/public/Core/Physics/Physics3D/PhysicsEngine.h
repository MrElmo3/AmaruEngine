#pragma once
#include <Core/Physics/APhysicsEngine.h>
#include <glm/glm.hpp>
#include <vector>

class AObject;
class ACollider3DComponent;
class Rigidbody3DComponent;

namespace Physics3D {

class PhysicObject;
class Octree;
/**
 * This class encasulates all the physics calculations for 3D objects.
 */
class PhysicsEngine : public APhysicsEngine {

private:
	struct CollisionManifold3D{
		glm::vec3 PenetrationVector;
		float Depth;
	};

	std::vector<PhysicObject*> physicObjects;

	Octree* octree;
	glm::vec3 worldMin = { -500, -500, -500 };
	glm::vec3 worldMax = { 500, 500, 500 };

public:
	PhysicsEngine();

protected:
	void RegisterObject(AObject* object) override;

	void MoveObjects() override;
	void MoveRigidbody(Rigidbody3DComponent* rigidbody);
	void UpdateAceleration(Rigidbody3DComponent* rigidbody);
	void UpdateVelocity(Rigidbody3DComponent* rigidbody);
	void UpdatePosition(Rigidbody3DComponent* rigidbody);

	void UpdateTree() override;
	void UpdateContainers();

	void CheckCollisions() override;
	bool CheckCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB);
	CollisionManifold EPA(ACollider* colliderA, ACollider* colliderB, std::vector<glm::vec3>& simplex) override;
	void ResolveCollision2D(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold);

private:
	void Resolve2RBCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold);
	void Resolve1RBCollision(PhysicObject* physicObject, CollisionManifold manifold);
};
}
