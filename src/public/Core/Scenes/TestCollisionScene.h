#pragma once
#include "ASceneController.h"

class EmptyObject;
class Square;
class TestColisionScene : public ASceneController {
public:
	TestColisionScene();

	EmptyObject* cameraObject;

	EmptyObject* circle;
	EmptyObject* floor;

	void Update(double deltaTime) override;
};
