#pragma once
#include "ASceneController.h"

class Square;
class TestColisionScene : public ASceneController{
public:
	TestColisionScene();

	Square* square1;
	
	void Update(double deltaTime) override;
};
