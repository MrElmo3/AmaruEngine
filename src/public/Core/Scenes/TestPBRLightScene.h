#pragma once
#include "ASceneController.h"

class PointLight;
class Cube;
class TestPBRLightScene : public ASceneController {

private:
	Cube* testCube;
	float value;

public:
	TestPBRLightScene();

	void Update(double deltaTime) override;
};
