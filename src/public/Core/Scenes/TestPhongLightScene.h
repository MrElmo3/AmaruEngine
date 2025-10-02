#pragma once
#include "ASceneController.h"

class PointLight;
class Cube;
class TestPhongLightScene : public ASceneController {

private:
	Cube* testCube;
	PointLight* light;
	float value;

public:
	TestPhongLightScene();

	void Update(double deltaTime) override;
};
