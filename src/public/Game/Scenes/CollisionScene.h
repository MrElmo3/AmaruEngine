#pragma once
#include "Core/Scenes/ASceneController.h"

class EmptyObject;
class CollisionScene : public ASceneController {
private:
	EmptyObject* cameraObject;

	EmptyObject* circle;

public:
	CollisionScene();
	void Update(double deltaTime) override;

private: 
	void CreateSphere();

	bool sphereCreated = false;
};