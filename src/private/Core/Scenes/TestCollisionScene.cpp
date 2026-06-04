#include <Core/Scenes/TestCollisionScene.h>

#include <Core/Global.h>
#include <Core/Components/Render/RenderQuadComponent.h>
#include <Core/Components/Physics/2D/SquareColliderComponent.h>
#include <Core/Components/Physics/2D/CircleColliderComponent.h>
#include <Core/Components/Physics/3D/BoxColliderComponent.h>
#include <Core/Components/Physics/3D/SphereColliderComponent.h>
#include <Core/Objects/2D/Square.h>
#include <Core/Objects/EmptyObject.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>
#include <Core/Components/Physics/3D/Rigidbody3DComponent.h>

TestColisionScene::TestColisionScene() {

	scenePhysicsType = PhysicsType::ENABLE_3D;

	cameraObject = (EmptyObject*)(new CameraObject("Camera", nullptr, this));
	auto* cameraComponent = ((CameraObject*)cameraObject)->GetCameraComponent();
	cameraObject->RotateEuler(glm::vec3(15, 0, 0));
	cameraObject->SetPosition(glm::vec3(0, 0, 20));
	cameraComponent->mainCamera = true;
	cameraComponent->orthographic = false;
	cameraComponent->orthoSize = 4.0f;

	// auto* testSquare = new Square("squareMouse", nullptr, this);
	// testSquare->GetRenderComponent()->enableRender = true;
	// // testSquare->AddComponent(new FollowMouseComponent(testSquare));
	// testSquare->AddComponent<Rigidbody2DComponent>();
	// testSquare->AddComponent<InputComponent>();
	// MovementComponent* movement =  testSquare->AddComponent<MovementComponent>();
	// testSquare->SetPosition(glm::vec3(-0.5, 0.5, 0);
	// testSquare->SetScale(glm::vec3(0.3f);
	// movement->SetSpeed(1);
	// movement->SetDirection(glm::vec2(1, -1));

	// square1 = new Square("staticSquare", nullptr, this);
	// square1->GetRenderComponent()->enableRender = false;
	// staticSquare->GetColliderComponent()->isTrigger = true;
	// staticSquare->AddComponent(new FollowMouseComponent(staticSquare));
	// square1->SetPosition(glm::vec3(0, 4, 0));
	// square1->SetScale(glm::vec3(0.5f));
	// square1->RotateEuler(glm::vec3(0.f, 0.f, 15));
	// auto addedComponent = square1->AddComponent<SquareColliderComponent>();
	// addedComponent->SetPosition(glm::vec2(0, 0.25f));
	// auto rigidbody = square1->AddComponent<Rigidbody2DComponent>();
	// rigidbody->SetGravityScale(0.5f);

	// auto childSquare = new Square("childSquare", square1);
	// childSquare->SetPosition(glm::vec3(2.5, -2, 0));
	// childSquare->RotateEuler(glm::vec3(0.f, 0.f, 45));
	// childSquare->GetRenderComponent()->enableRender = false;

	// squareFloor = new Square("squareFloor", nullptr, this);
	// squareFloor->SetPosition(glm::vec3(0, -4, 0));
	// squareFloor->RotateEuler(glm::vec3(0, 0, 0));
	// squareFloor->SetScale(glm::vec3(20, 1, 1));
	// squareFloor->GetRenderComponent()->enableRender = false;

	
	// squareFloor = new Square("squareFloor", nullptr, this);
	// squareFloor->SetPosition(glm::vec3(-6, 0, 0));
	// squareFloor->RotateEuler(glm::vec3(0, 0, 90));
	// squareFloor->SetScale(glm::vec3(20, 1, 1));
	// squareFloor->GetRenderComponent()->enableRender = false;

	floor = new EmptyObject("floor", nullptr, this);
	floor->SetPosition(glm::vec3(0, -6, 0));
	floor->RotateEuler(glm::vec3(0, 0, 0));
	floor->SetScale(glm::vec3(20, 1, 20));
	BoxColliderComponent* floorCollider = floor->AddComponent<BoxColliderComponent>();
	// floor->GetRenderComponent()->enableRender = false;


	//Tree  test

	// square1 = new Square("square 1", nullptr, this);
	// square1->GetRenderComponent()->enableRender = false;
	// staticSquare->GetColliderComponent()->isTrigger = true;
	// staticSquare->AddComponent(new FollowMouseComponent(staticSquare));
	// square1->SetPosition(glm::vec3(2.5, 2, 0));
	// square1->SetScale(glm::vec3(0.5f));

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

	// circle = new EmptyObject("Circle", nullptr, this);
	// CircleColliderComponent* colliderComponent = circle->AddComponent<CircleColliderComponent>();
	// circle->SetPosition(glm::vec3(0, 4, 0));
	// // circleObject->SetScale(glm::vec3(0.5f));
	// auto rigidbody = circle->AddComponent<Rigidbody2DComponent>();
	// rigidbody->gravityScale = 0.5f;
	// rigidbody->AddForce(glm::vec2(150.f, 0.f));

	// square1 = new Square("square 2", nullptr, this);
	// square1->GetRenderComponent()->enableRender = false;
	// // staticSquare->GetColliderComponent()->isTrigger = true;
	// // staticSquare->AddComponent(new FollowMouseComponent(staticSquare));
	// square1->SetPosition(glm::vec3(-2.5, 2, 0));
	// square1->SetScale(glm::vec3(0.5f));

	// square1 = new Square("square 3", nullptr, this);
	// square1->GetRenderComponent()->enableRender = false;
	// // staticSquare->GetColliderComponent()->isTrigger = true;
	// // staticSquare->AddComponent(new FollowMouseComponent(staticSquare));
	// square1->SetPosition(glm::vec3(-2.5, -2, 0));
	// square1->SetScale(glm::vec3(0.5f));
}

void TestColisionScene::Update(double deltaTime) {
	// cameraObject->RotateEuler(glm::vec3(0,0, ));

	// square1->position += glm::vec3(deltaTime * -1.f, 0.f, 0.f);

	// std::cout<<deltaTime<<'\n';

	ASceneController::Update(deltaTime);

}