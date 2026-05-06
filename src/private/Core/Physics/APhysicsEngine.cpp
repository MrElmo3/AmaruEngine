#include <Core/Physics/APhysicsEngine.h>
#include <Core/Objects/AObject.h>

void APhysicsEngine::Awake(std::vector<AObject*> objects){
	for (auto* object : objects) {
		RegisterObject(object);
	}
}

void APhysicsEngine::UpdatePhysics() {
	MoveObjects();
	UpdateTree();
	CheckCollisions();
}