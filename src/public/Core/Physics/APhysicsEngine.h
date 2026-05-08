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
	virtual void RegisterObject(AObject* object) {};
	virtual void MoveObjects() {};
	virtual void UpdateTree() {};
	virtual void CheckCollisions() {};
	bool GJKCheck(ACollider* colliderA, ACollider* colliderB);

private:
	bool HandleSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction);
	bool LineCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
	bool TriangleCase(std::vector<glm::vec3>& simplex, glm::vec3& direction);
};