#pragma once
#include "ASceneController.h"

class EmptyObject;
class Square;
class TestColisionScene : public ASceneController {
public:
	TestColisionScene();

	EmptyObject* circle;
	Square* squareFloor;

	void Update(double deltaTime) override;
};
