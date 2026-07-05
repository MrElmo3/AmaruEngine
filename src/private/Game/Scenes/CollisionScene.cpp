#include <Game/Scenes/CollisionScene.h>

#include <Core/Objects/EmptyObject.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Global.h>

#include <Core/Components/Physics/2D/SquareColliderComponent.h>
#include <Core/Components/Physics/2D/CircleColliderComponent.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>


CollisionScene::CollisionScene() {

	srand(std::time(0));

	scenePhysicsType = PhysicsType::ENABLE_2D;

	cameraObject = (EmptyObject*)(new CameraObject("Camera", nullptr, this));
	auto* cameraComponent = ((CameraObject*)cameraObject)->GetCameraComponent();

	cameraObject->SetPosition(glm::vec3(0, 0, 1));
	cameraComponent->mainCamera = true;
	cameraComponent->orthographic = true;
	cameraComponent->orthoSize = 15.0f;

	//Floors and walls
	EmptyObject* wall1 = new EmptyObject("floor", nullptr, this);
	wall1->SetPosition(glm::vec3(0, -11, 0));
	wall1->SetScale(glm::vec3(30, 5, 1));
	SquareColliderComponent* wall1collider = wall1->AddComponent<SquareColliderComponent>();

	EmptyObject* wall2 = new EmptyObject("floor", nullptr, this);
	wall2->SetPosition(glm::vec3(0, 11, 0));
	wall2->SetScale(glm::vec3(30, 5, 1));
	SquareColliderComponent* wall2Collider = wall2->AddComponent<SquareColliderComponent>();

	EmptyObject* wall3 = new EmptyObject("floor", nullptr, this);
	wall3->SetPosition(glm::vec3(16, 0, 0));
	wall3->SetScale(glm::vec3(5, 20, 1));
	SquareColliderComponent* wall3Collider = wall3->AddComponent<SquareColliderComponent>();

	EmptyObject* wall4 = new EmptyObject("floor", nullptr, this);
	wall4->SetPosition(glm::vec3(-16, 0, 0));
	wall4->SetScale(glm::vec3(5, 20, 1));
	SquareColliderComponent* wall4Collider = wall4->AddComponent<SquareColliderComponent>();
}

void CollisionScene::Update(double deltaTime) {
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z) == GLFW_PRESS) {
		CreateSphere();
	}
	else {
		sphereCreated = false;
	}
	ASceneController::Update(deltaTime);
}

void CollisionScene::CreateSphere() {
	if(sphereCreated) return;
	sphereCreated = true;

	for (int i = 0; i < 300; i++) {
		glm::vec3 randomPosition = glm::normalize(glm::vec3(rand(), rand(), 0)) - glm::vec3(0.5f, 0.5f, 0);
		randomPosition *= 6;
		glm::vec2 randomInitialVelocity = glm::normalize(glm::vec2(rand(), rand())) - glm::vec2(0.5f, 0.5f);
		randomInitialVelocity *= 20;
		EmptyObject* sphere = new EmptyObject("Sphere", nullptr, this);
		sphere->SetPosition(randomPosition);
		auto collider = sphere->AddComponent<CircleColliderComponent>();
		collider->SetRadius(0.1f);
		auto rb = sphere->AddComponent<Rigidbody2DComponent>();
		rb->velocity = randomInitialVelocity;
		rb->gravityScale = 0;
	}
}
