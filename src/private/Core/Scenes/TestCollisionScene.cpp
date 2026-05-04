#include <Core/Scenes/TestCollisionScene.h>

#include <Core/Global.h>
#include <Core/Components/Render/RenderQuadComponent.h>
#include <Core/Components/Physics/2D/SquareColliderComponent.h>
#include <Core/Objects/2D/Square.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>

TestColisionScene::TestColisionScene() {

	scenePhysicsType = PhysicsType::ENABLE_2D;

	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(0, 0, 2);
	cameraComponent->mainCamera = true;
	cameraComponent->orthographic = true;
	cameraComponent->orthoSize = 4.0f;
	
	// auto* testSquare = new Square("squareMouse", nullptr, this);
	// testSquare->GetRenderComponent()->enableRender = true;
	// // testSquare->AddComponent(new FollowMouseComponent(testSquare));
	// testSquare->AddComponent<Rigidbody2DComponent>();
	// testSquare->AddComponent<InputComponent>();
	// MovementComponent* movement =  testSquare->AddComponent<MovementComponent>();
	// testSquare->position = glm::vec3(-0.5, 0.5, 0);
	// testSquare->scale = glm::vec3(0.3f);
	// movement->SetSpeed(1);
	// movement->SetDirection(glm::vec2(1, -1));

	square1 = new Square("staticSquare", nullptr, this);
	square1->GetRenderComponent()->enableRender = false;
	// staticSquare->GetColliderComponent()->isTrigger = true;
	// staticSquare->AddComponent(new FollowMouseComponent(staticSquare));
	square1->position = glm::vec3(0, 4, 0);
	square1->scale = glm::vec3(0.5f);
	// auto addedComponent = square1->AddComponent<SquareColliderComponent>();
	// addedComponent->SetPosition(glm::vec2(0, 0.25f));
	auto rigidbody = square1->AddComponent<Rigidbody2DComponent>();

	squareFloor = new Square("squareFloor", nullptr, this);
	squareFloor->position = glm::vec3(0, -2, 0);
	squareFloor->scale = glm::vec3(5, 1, 1);
	squareFloor->GetRenderComponent()->enableRender = false;
}

void TestColisionScene::Update(double deltaTime) {

	// square1->position += glm::vec3(deltaTime * -1.f, 0.f, 0.f);

	// std::cout<<deltaTime<<'\n';
	
	ASceneController::Update(deltaTime);

}