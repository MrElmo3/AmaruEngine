#pragma once
#include "Core/Scenes/ASceneController.h"

class Maze3DScene : public ASceneController {
private:
public:
	Maze3DScene();
	void Update(double deltaTime) override;
};