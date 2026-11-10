#pragma once
#include <Core/Physics/APhysicsEngine.h>
#include <glm/glm.hpp>
#include <unordered_set>
#include <vector>

class AObject;
class ACollider3DComponent;
class Rigidbody3DComponent;

namespace Physics3D {

class PhysicObject;
class Octree;
/**
 * This class encasulates all the physics calculations for 3D objects.
 * Only linear dynamics are simulated (no rotation / torque).
 */
class PhysicsEngine : public APhysicsEngine {

private:
	/// @brief A point of the Minkowski difference (A - B) plus the points on each shape that generated it.
	struct SupportPoint {
		glm::vec3 point;
		glm::vec3 onA;
		glm::vec3 onB;
	};

	/// @brief Simplex used by GJK. The newest point is always at index 0.
	struct Simplex {
		SupportPoint points[4];
		int size = 0;
	};

	std::vector<PhysicObject*> physicObjects;

	/// @brief Objects already owned by a PhysicObject (avoids registering the same object twice).
	std::unordered_set<AObject*> registeredObjects;

	Octree* octree;

public:
	PhysicsEngine();
	~PhysicsEngine() override;

	void RegisterObject(AObject* object) override;

protected:
	void MoveObjects() override;
	void MoveRigidbody(Rigidbody3DComponent* rigidbody);

	void UpdateTree() override;

	void CheckCollisions() override;
	void CheckCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB);
	void ResolveCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB, const CollisionManifold& manifold);

private:
	static SupportPoint Support(ACollider3DComponent* colliderA, ACollider3DComponent* colliderB, glm::vec3 direction);

	/// @brief GJK intersection test. When it returns true, simplex is a tetrahedron that contains the origin.
	bool GJK(ACollider3DComponent* colliderA, ACollider3DComponent* colliderB, Simplex& simplex);
	bool DoSimplex(Simplex& simplex, glm::vec3& direction);
	bool LineCase(Simplex& simplex, glm::vec3& direction);
	bool TriangleCase(Simplex& simplex, glm::vec3& direction);
	bool TetrahedronCase(Simplex& simplex, glm::vec3& direction);

	/// @brief Expanding Polytope Algorithm. The returned normal points from A to B.
	CollisionManifold EPA3D(ACollider3DComponent* colliderA, ACollider3DComponent* colliderB, const Simplex& simplex);
};
}
