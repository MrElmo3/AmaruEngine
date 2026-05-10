#pragma once
#include <vector>
#include <glm/glm.hpp>

enum class PhysicsType {
	DISABLED = 0,
	ENABLE_2D = 1,
	ENABLE_3D = 2
};

class AObject;
class ACollider;
class APhysicsEngine {

public:
	virtual void Awake(std::vector<AObject*> objects);
	virtual void UpdatePhysics();

protected:

	struct GJKResult {
		bool Collide;
		std::vector<glm::vec3>* Simplex;
	};

	struct CollisionManifold {
		glm::vec3 PenetrationVector;
		float Depth;
	};

	virtual void RegisterObject(AObject* object) {};
	virtual void MoveObjects() {};
	virtual void UpdateTree() {};
	virtual void CheckCollisions() {};
	GJKResult GJKCheck(ACollider* colliderA, ACollider* colliderB);
	CollisionManifold EPA(ACollider* colliderA, ACollider* colliderB, std::vector<glm::vec3>* simplex);

private:
	bool HandleSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction);
	bool LineCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
	bool TriangleCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
	bool TetrahedronCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
};