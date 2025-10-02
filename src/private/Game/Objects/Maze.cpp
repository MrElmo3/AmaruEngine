#include "Game/Objects/Maze.h"
#include "Game/Components/MazeGeneratorComponent.h"

Maze::Maze(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {

	mazeGeneratorComponent = AddComponent<MazeGeneratorComponent>();
	// rotation = glm::quat(1.0f, 0, 1.0f, 1.0f);
}

Maze::~Maze() {
	delete mazeGeneratorComponent;
}

