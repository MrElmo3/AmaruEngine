#pragma once
#include <vector>
#include <Util/Singleton.h>
#include <glm/glm.hpp>

enum class PhysicsType {
	DISABLED = 0,
	ENABLE_2D = 1,
	ENABLE_3D = 2
};

class AObject;
class APhysicsEngine{
public:
	virtual void Awake(std::vector<AObject*> objects);
	virtual void UpdatePhysics();

private:
	virtual void RegisterObject(AObject* object) {};
	virtual void MoveObjects() {};
	virtual void UpdateTree() {};
	virtual void CheckCollisions() {};
};