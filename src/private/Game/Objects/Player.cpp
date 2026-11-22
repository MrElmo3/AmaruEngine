#include "Game/Objects/Player.h"

#include "Core/Components/Render/RenderQuadComponent.h"
#include "Core/Components/Input/InputComponent.h"
#include "Core/Components/Physics/3D/SphereColliderComponent.h"
#include "Game/Components/MazeGeneratorComponent.h"
#include "Game/Components/MovementComponent.h"
#include "Core/Components/Physics/3D/Rigidbody3DComponent.h"
#include "Core/Components/Render/CameraComponent.h"
#include "Core/Components/Render/Lights/LightComponent.h"
#include "Core/Scenes/ASceneController.h"
#include "Game/Objects/Maze.h"


Player::Player(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {

	playerCamera = AddComponent<CameraComponent>();
	playerCamera->orthographic = false;
	playerCamera->mainCamera = true;
	playerCamera->FOV = 90.0f;
	
	rigidbodyComponent = AddComponent<Rigidbody3DComponent>();
	// Sphere so the player slides smoothly along walls and corners.
	colliderComponent = AddComponent<SphereColliderComponent>();
	colliderComponent->SetRadius(0.25f);
	inputComponent = AddComponent<InputComponent>();
	movementComponent = AddComponent<MovementComponent>();

	// Flashlight: the light takes the player position and forward every frame,
	// so it always points where the camera looks.
	flashlight = AddComponent<LightComponent>();
	flashlight->SetType(LightType::SPOT)
		->SetSpotRange(20.0f, 30.0f) // inner / outer cone angle in degrees
		->SetRange(15.0f)
		->SetColor(3.0f, 2.8f, 2.4f); // slightly warm, above 1 to survive the tonemapping
	
	// TextureMaterial* baseMaterial = dynamic_cast<TextureMaterial*>(renderComponent->material);
	// baseMaterial->color = glm::vec3(63, 72, 204) / 255.0f;
	
	rigidbodyComponent->gravityScale = 0;
	// No bounce and no friction: hitting a wall just stops/slides the player.
	rigidbodyComponent->restitution = 0.0f;
	rigidbodyComponent->friction = 0.0f;
}

void Player::Awake() {

	position = spawnPosition;
	RotateEuler(glm::vec3(0.0f, 180.0f, 0.0f));

	movementComponent->SetSpeed(2.0f);
	movementComponent->SetLookSensitivity(0.1f); // degrees per pixel

	AObject::Awake();
}

Player::~Player() {
	delete playerCamera;
	delete rigidbodyComponent;
	delete colliderComponent;
	delete inputComponent;
	delete movementComponent;
	// The scene keeps a pointer to the light: remove it before deleting.
	scene->activeLights.erase(flashlight);
	delete flashlight;
}