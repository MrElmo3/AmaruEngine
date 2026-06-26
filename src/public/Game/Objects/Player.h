#pragma once
#include <Core/Objects/AObject.h>

class CameraComponent;
class InputComponent;
class MovementComponent;
class Rigidbody3DComponent;
class BoxColliderComponent;

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
	BoxColliderComponent* colliderComponent;

public:
	CameraComponent* GetCamera() const { return playerCamera; }
};