#pragma once
#include "Core/Scenes/ASceneController.h"

class EmptyObject;
class CollisionScene : public ASceneController {
private:
	EmptyObject* cameraObject;

	EmptyObject* circle;

public:
	CollisionScene();
};