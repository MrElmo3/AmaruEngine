#pragma once
#include "ASceneController.h"

class PointLight;
class TestPhongLightScene : public ASceneController {

private:
	PointLight* light;
	float value;

public:
	TestPhongLightScene();

	void Update(double deltaTime) override;
};
