#pragma once
#include <Core/Objects/AObject.h>

class CameraComponent;
class InputComponent;
class LightComponent;
class MovementComponent;
class Rigidbody3DComponent;
class SphereColliderComponent;

class Player : public AObject {
public:
	Player(const std::string &_name, AObject* _parent = nullptr, ASceneController* _scene = nullptr);
	~Player() override;

	void Awake() override;
	
private:

	CameraComponent* playerCamera;

	Rigidbody3DComponent* rigidbodyComponent;
	InputComponent* inputComponent;
	MovementComponent* movementComponent;
	SphereColliderComponent* colliderComponent;
	// Spot light that follows the camera, used as a flashlight.
	LightComponent* flashlight;

	glm::vec3 spawnPosition = glm::vec3(0.0f);

public:
	CameraComponent* GetCamera() const { return playerCamera; }

	/// @brief Position where the player appears when the scene starts.
	void SetSpawnPosition(glm::vec3 position) { spawnPosition = position; }
};