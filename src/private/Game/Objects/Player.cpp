#include "Game/Objects/Player.h"

#include "Core/Components/Render/RenderQuadComponent.h"
#include "Core/Components/Input/InputComponent.h"
#include "Core/Components/Physics/3D/SphereColliderComponent.h"
#include "Game/Components/MazeGeneratorComponent.h"
#include "Game/Components/MovementComponent.h"
#include "Core/Components/Physics/3D/Rigidbody3DComponent.h"
#include "Core/Components/Render/CameraComponent.h"
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
	movementComponent->SetRotationSpeed(60.0f); // degrees per second

	AObject::Awake();
}

Player::~Player() {
	delete playerCamera;
	delete rigidbodyComponent;
	delete colliderComponent;
	delete inputComponent;
	delete movementComponent;
}