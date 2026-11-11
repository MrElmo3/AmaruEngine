#include "Game/Scenes/Maze3DScene.h"

#include "Core/Objects/EmptyObject.h"
#include "Core/Objects/Light/DirectionalLight.h"
#include "Game/Components/MazeGeneratorComponent.h"
#include "Game/Objects/Maze.h"
#include "Game/Objects/Player.h"
#include "Util/Logger.h"
#include <glm/gtx/string_cast.hpp>

Maze3DScene::Maze3DScene() {

	// Walls are static box colliders and the player is a sphere rigidbody.
	scenePhysicsType = PhysicsType::ENABLE_3D;

	// EmptyObject* player = new EmptyObject(nullptr, "Camera");
	// CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(
	// 	player->AddComponent(new CameraComponent(player))
	// 	);
	// cameraComponent->orthographic = true;
	// cameraComponent->mainCamera = true;
	// cameraComponent->orthoSize = 1.0f;
	// player->position = glm::vec3(0, 0, 3);
	// AddObject(player);

	auto* maze = new Maze("Maze", nullptr, this);
	// AddObject(maze);
	
	auto* player = new Player("Player", nullptr, this);
	// The start cell only depends on the size, so it is known before the maze is generated.
	player->SetSpawnPosition(maze->GetMazeGeneratorComponent()->GetStartPosition());
	// AddObject(player);

	a = new DirectionalLight("Light", nullptr, this);
}

void Maze3DScene::Update(double deltaTime) {
	Logger::Log(glm::to_string(a->GetPosition()));
	ASceneController::Update(deltaTime);
}