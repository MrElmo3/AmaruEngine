#include "Game/Scenes/Maze3DScene.h"

#include <cmath>

#include "Core/Global.h"
#include "Core/Components/Render/ModelRendererComponent.h"
#include "Core/Materials/LitMaterial.h"
#include "Core/Objects/EmptyObject.h"
#include "Core/Objects/Light/DirectionalLight.h"
#include "Core/Render/Model.h"
#include "Core/Render/Render.h"
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
	mazeGenerator = maze->GetMazeGeneratorComponent();
	// AddObject(maze);

	auto* player = new Player("Player", nullptr, this);
	// The start cell only depends on the size, so it is known before the maze is generated.
	player->SetSpawnPosition(mazeGenerator->GetStartPosition());
	// AddObject(player);

	CreateTrophy();

	// auto* light = new DirectionalLight("Light", nullptr, this);
	// Tilted down and sideways so the walls cast shadows over each other.
	// light->RotateEuler(glm::vec3(-50.0f, 210.0f, 0.0f));
	// light->SetColor(3.0f, 3.0f, 3.0f);
	// a = light;
}

void Maze3DScene::CreateTrophy() {
	trophy = new EmptyObject("Trophy", nullptr, this);
	auto* trophyModel = new EmptyObject("TrophyModel", trophy);

	auto* material = new LitMaterial();
	material
		->SetAlbedo("Assets/Textures/1001_albedo.jpg")
		->SetNormal("Assets/Textures/1001_normal.png")
		->SetRoughness("Assets/Textures/1001_roughness.jpg")
		->SetAO("Assets/Textures/1001_AO.jpg")
		->IsPBR(true)
		->shader = Render::GetInstance().CreateShader(
			"Assets/Shaders/PBR/PBR.vert",
			"Assets/Shaders/PBR/PBR.frag"
		);

	auto* model = new Model("Assets/Models/Survival_BackPack_2.fbx");
	auto* renderer = trophyModel->AddComponent<ModelRendererComponent>();
	renderer->material = material;
	renderer->SetModel(model);

	// The file's units and pivot are unknown: measure it, scale its largest side
	// to TROPHY_SIZE and move it so the bottom center sits on the trophy origin.
	glm::vec3 min, max;
	model->GetBounds(min, max);
	glm::vec3 extent = max - min;
	float largestSide = glm::max(extent.x, glm::max(extent.y, extent.z));
	float scale = largestSide > 0.0f ? TROPHY_SIZE / largestSide : 1.0f;

	glm::vec3 bottomCenter = glm::vec3((min.x + max.x) * 0.5f, min.y, (min.z + max.z) * 0.5f);
	trophyModel->SetScale(glm::vec3(scale));
	trophyModel->SetPosition(-bottomCenter * scale);
}

void Maze3DScene::Awake() {
	// Awakes every object, which generates the maze, so the end cell is known after this.
	ASceneController::Awake();

	// Walls are 1 unit cubes centered at y = 0: the floor level is -0.5.
	trophyBasePosition = mazeGenerator->GetEndPosition() + glm::vec3(0.0f, -0.5f + TROPHY_BOB_HEIGHT, 0.0f);
	trophy->SetPosition(trophyBasePosition);
}

void Maze3DScene::Update(double deltaTime) {
	// Spin and bob so it reads as a goal item, not as part of the maze.
	trophy->RotateEuler(glm::vec3(0.0f, TROPHY_SPIN_SPEED * (float)deltaTime, 0.0f));
	float bob = TROPHY_BOB_HEIGHT * std::sin((float)Global::TIME * TROPHY_BOB_SPEED);
	trophy->SetPosition(trophyBasePosition + glm::vec3(0.0f, bob, 0.0f));

	ASceneController::Update(deltaTime);
}
