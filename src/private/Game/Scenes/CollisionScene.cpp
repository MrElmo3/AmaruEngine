#include <Game/Scenes/CollisionScene.h>

#include <Core/Objects/EmptyObject.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Global.h>

#include <Core/Components/Physics/3D/BoxColliderComponent.h>
#include <Core/Components/Physics/3D/SphereColliderComponent.h>
#include <Core/Components/Physics/3D/Rigidbody3DComponent.h>

#include <Game/Objects/Player.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>


CollisionScene::CollisionScene() {

	srand(std::time(0));

	scenePhysicsType = PhysicsType::ENABLE_3D;

	cameraObject = (EmptyObject*)(new Player("Camera", nullptr, this));
	cameraObject->SetPosition(glm::vec3(0, 0, 20));

	//Floors and walls
	EmptyObject* floor = new EmptyObject("floor", nullptr, this);
	floor->SetPosition(glm::vec3(0, -6, 0));
	floor->RotateEuler(glm::vec3(0, 0, 0));
	floor->SetScale(glm::vec3(20, 1, 20));
	BoxColliderComponent* floorCollider = floor->AddComponent<BoxColliderComponent>();	

	EmptyObject* wall1 = new EmptyObject("floor", nullptr, this);
	wall1->SetPosition(glm::vec3(-9.5, 3.5, 0));
	wall1->RotateEuler(glm::vec3(0, 0, 0));
	wall1->SetScale(glm::vec3(1, 20, 20));
	BoxColliderComponent* wallCollider1 = wall1->AddComponent<BoxColliderComponent>();

	EmptyObject* wall2 = new EmptyObject("floor", nullptr, this);
	wall2->SetPosition(glm::vec3(9.5, 3.5, 0));
	wall2->RotateEuler(glm::vec3(0, 0, 0));
	wall2->SetScale(glm::vec3(1, 20, 20));
	BoxColliderComponent* wallCollider2 = wall2->AddComponent<BoxColliderComponent>();

	EmptyObject* wall3 = new EmptyObject("floor", nullptr, this);
	wall3->SetPosition(glm::vec3(0, 3.5, 9.5));
	wall3->RotateEuler(glm::vec3(0, 0, 0));
	wall3->SetScale(glm::vec3(20, 20, 1));
	BoxColliderComponent* wallCollider3 = wall3->AddComponent<BoxColliderComponent>();

	EmptyObject* wall4 = new EmptyObject("floor", nullptr, this);
	wall4->SetPosition(glm::vec3(0, 3.5, -9.5));
	wall4->RotateEuler(glm::vec3(0, 0, 0));
	wall4->SetScale(glm::vec3(20, 20, 1));
	BoxColliderComponent* wallCollider4 = wall4->AddComponent<BoxColliderComponent>();


	//Spheres

	glm::vec3 randomPosition = glm::normalize(glm::vec3(rand(), 0, rand()));
		randomPosition *= 3;
		randomPosition.y = 6;
		glm::vec3 randomInitialVelocity = glm::normalize(glm::vec3(0, 0, 10));
		randomInitialVelocity *= 5;

		EmptyObject* sphere = new EmptyObject("Sphere", nullptr, this);
		sphere->AddComponent<SphereColliderComponent>();
		sphere->SetPosition(randomPosition);
		sphere->SetScale(glm::vec3(0.25f));
		auto rb = sphere->AddComponent<Rigidbody3DComponent>();
		rb->velocity = randomInitialVelocity;

	// for (int i = 0; i <= 1; i++) {
	// 	glm::vec3 randomPosition = glm::normalize(glm::vec3(rand(), 0, rand()));
	// 	randomPosition *= 3;
	// 	randomPosition.y = 6;
	// 	glm::vec3 randomInitialVelocity = glm::normalize(glm::vec3(rand(), 0, rand()));
	// 	randomInitialVelocity *= 3;

	// 	EmptyObject* sphere = new EmptyObject("Sphere", nullptr, this);
	// 	sphere->AddComponent<SphereColliderComponent>();
	// 	sphere->SetPosition(randomPosition);
	// 	sphere->SetScale(glm::vec3(0.25f));
	// 	auto rb = sphere->AddComponent<Rigidbody3DComponent>();
	// 	rb->velocity = randomInitialVelocity;
	// }

	time = 0;
}

void CollisionScene::Update(double deltaTime) {
	// if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z) == GLFW_PRESS) {
	// 	CreateSphere();
	// }
	ASceneController::Update(deltaTime);
}

void CollisionScene::CreateSphere() {
	EmptyObject* sphere = new EmptyObject("Sphere", nullptr, this);
	sphere->AddComponent<SphereColliderComponent>();
	sphere->SetPosition(glm::vec3(0, 10, 0));
	sphere->SetScale(glm::vec3(0.25f));
	sphere->AddComponent<Rigidbody3DComponent>();
}