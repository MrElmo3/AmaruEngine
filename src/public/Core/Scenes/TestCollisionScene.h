#pragma once
#include "ASceneController.h"

class Square;
class TestColisionScene : public ASceneController{
public:
	TestColisionScene();

	Square* square1;
	Square* squareFloor;
	
	void Update(double deltaTime) override;
};
