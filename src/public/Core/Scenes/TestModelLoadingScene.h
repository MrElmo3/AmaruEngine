#pragma once
#include "Core/Scenes/ASceneController.h"

class EmptyObject;

class TestModelLoadingScene : public ASceneController {
private:
	EmptyObject* testModel;
	float value;

public:
	TestModelLoadingScene();
	void Update(double deltaTime) override;
};