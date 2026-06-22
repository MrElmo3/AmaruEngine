#include <Game/Scenes/CollisionScene.h>

#include <Core/Objects/EmptyObject.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Global.h>

#include <Core/Components/Physics/3D/BoxColliderComponent.h>
#include <Core/Components/Physics/3D/SphereColliderComponent.h>

#include <Core/Components/Physics/3D/Rigidbody3DComponent.h>


CollisionScene::CollisionScene() {

	scenePhysicsType = PhysicsType::ENABLE_3D;

	cameraObject = (EmptyObject*)(new CameraObject("Camera", nullptr, this));
	auto* cameraComponent = ((CameraObject*)cameraObject)->GetCameraComponent();
	cameraObject->RotateEuler(glm::vec3(15, 0, 0));
	cameraObject->SetPosition(glm::vec3(0, 0, 20));
	cameraComponent->mainCamera = true;
	cameraComponent->orthographic = false;
	cameraComponent->orthoSize = 4.0f;

	EmptyObject* floor = new EmptyObject("floor", nullptr, this);
	floor->SetPosition(glm::vec3(0, -6, 0));
	floor->RotateEuler(glm::vec3(0, 0, 0));
	floor->SetScale(glm::vec3(20, 1, 20));
	BoxColliderComponent* floorCollider = floor->AddComponent<BoxColliderComponent>();


	EmptyObject* wall1 = new EmptyObject("floor", nullptr, this);
	wall1->SetPosition(glm::vec3(-10, 0, 0));
	wall1->RotateEuler(glm::vec3(0, 0, 0));
	wall1->SetScale(glm::vec3(1, 20, 20));
	BoxColliderComponent* wallCollider1 = wall1->AddComponent<BoxColliderComponent>();
	

	circle = new EmptyObject("Sphere", nullptr, this);
	SphereColliderComponent* colliderComponent2 = circle->AddComponent<SphereColliderComponent>();
	circle->SetPosition(glm::vec3(0, 6, 0));
	// circleObject->SetScale(glm::vec3(0.5f));
	auto rigidbody2 = circle->AddComponent<Rigidbody3DComponent>();
	rigidbody2->gravityScale = 0.5f;


	circle = new EmptyObject("Sphere", nullptr, this);
	SphereColliderComponent* colliderComponent1 = circle->AddComponent<SphereColliderComponent>();
	circle->SetPosition(glm::vec3(0, 20, 0));
	// circleObject->SetScale(glm::vec3(0.5f));
	auto rigidbody1 = circle->AddComponent<Rigidbody3DComponent>();


}