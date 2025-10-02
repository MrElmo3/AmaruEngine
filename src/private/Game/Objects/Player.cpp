#include "Game/Objects/Player.h"

#include "Core/Components/Render/RenderQuadComponent.h"
#include "Core/Components/Input/InputComponent.h"
#include "Core/Components/Physics/3D/BoxColliderComponent.h"
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
	playerCamera->FOV = 60.0f;
	
	rigidbodyComponent = AddComponent<Rigidbody3DComponent>();
	colliderComponent = AddComponent<BoxColliderComponent>()
		->SetCenter(glm::vec3(0.0f))
		->SetHalfSize(glm::vec3(0.2f));
	inputComponent = AddComponent<InputComponent>();
	movementComponent = AddComponent<MovementComponent>();
	
	// TextureMaterial* baseMaterial = dynamic_cast<TextureMaterial*>(renderComponent->material);
	// baseMaterial->color = glm::vec3(63, 72, 204) / 255.0f;
	
}

void Player::Awake() {
	
	position = glm::vec3(0.0f, 1.5f, -5.0f);
	RotateEuler(glm::vec3(0.0f, 180.0f, 0.0f));

	movementComponent->SetSpeed(2.0f);
	movementComponent->SetRotationSpeed(5.0f);

	AObject::Awake();
}

Player::~Player() {
	delete playerCamera;
	delete rigidbodyComponent;
	delete colliderComponent;
	delete inputComponent;
	delete movementComponent;
}