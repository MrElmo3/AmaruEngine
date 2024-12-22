#pragma once
#include "Core/Scenes/ASceneController.h"

class TestScene : public ASceneController {
public:
	TestScene();

	void Update(double deltaTime) override;
};