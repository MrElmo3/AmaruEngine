#pragma once
#include <glm/glm.hpp>

#include "Core/Scenes/ASceneController.h"

class AObject;
class MazeGeneratorComponent;

class Maze3DScene : public ASceneController {
private:
	MazeGeneratorComponent* mazeGenerator = nullptr;

	// Showcase model placed at the end of the maze. The root spins and bobs,
	// its child holds the model, centered on the root.
	AObject* trophy = nullptr;
	glm::vec3 trophyBasePosition = glm::vec3(0.0f);

	// Largest side of the model once fitted, smaller than a 1 unit corridor.
	const float TROPHY_SIZE = 0.5f;
	const float TROPHY_SPIN_SPEED = 45.0f;	// degrees per second
	const float TROPHY_BOB_HEIGHT = 0.05f;
	const float TROPHY_BOB_SPEED = 2.0f;	// radians per second

public:
	Maze3DScene();
	void Awake() override;
	void Update(double deltaTime) override;

private:
	void CreateTrophy();
};
