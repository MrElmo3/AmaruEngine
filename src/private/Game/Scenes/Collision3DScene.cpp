#include <Game/Scenes/Collision3DScene.h>

#include <Core/Objects/EmptyObject.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Global.h>

#include <Core/Components/Physics/3D/BoxColliderComponent.h>
#include <Core/Components/Physics/3D/SphereColliderComponent.h>
#include <Core/Components/Physics/3D/Rigidbody3DComponent.h>
#include <Core/Components/Input/InputComponent.h>
#include <Game/Components/MovementComponent.h>

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>

namespace {
	// Inner half-extents of the box the spheres bounce around in.
	const glm::vec3 BOX_HALF_SIZE = glm::vec3(13.5f, 8.5f, 8.f);
	const float WALL_THICKNESS = 5.f;

	const int SPHERE_COUNT = 10;
	const float SPHERE_RADIUS = 0.2f;
	const float SPAWN_RADIUS = 6.f;
	const float INITIAL_SPEED = 20.f;

	/// @brief Uniform random float in [-1, 1].
	float RandomSigned() {
		return (rand() / (float)RAND_MAX) * 2.f - 1.f;
	}

	/// @brief Random unit vector (rejection sampling inside the unit sphere).
	glm::vec3 RandomDirection() {
		glm::vec3 v;
		float lengthSq;
		do {
			v = glm::vec3(RandomSigned(), RandomSigned(), RandomSigned());
			lengthSq = glm::dot(v, v);
		} while (lengthSq > 1.f || lengthSq < 1e-4f);
		return v / glm::sqrt(lengthSq);
	}
}

Collision3DScene::Collision3DScene() {

	srand((unsigned int)std::time(nullptr));

	scenePhysicsType = PhysicsType::ENABLE_3D;

	cameraObject = (EmptyObject*)(new CameraObject("Camera", nullptr, this));
	auto* cameraComponent = ((CameraObject*)cameraObject)->GetCameraComponent();

	cameraObject->SetPosition(glm::vec3(0, 0, 40));
	cameraComponent->mainCamera = true;
	cameraComponent->orthographic = false;

	cameraObject->AddComponent<InputComponent>();
	auto* movement = cameraObject->AddComponent<MovementComponent>();
	auto* rb = cameraObject->AddComponent<Rigidbody3DComponent>();
	rb->gravityScale = 0;
	movement->SetSpeed(5);
	movement->SetRotationSpeed(5);

	// Closed box: floor, ceiling, right, left, back, front.
	// Each wall is WALL_THICKNESS thick and sits just outside the inner volume.
	const glm::vec3 h = BOX_HALF_SIZE;
	const float t = WALL_THICKNESS;
	const float off = t * 0.5f;
	const glm::vec3 outer = 2.f * h + glm::vec3(2.f * t);

	CreateWall(glm::vec3(0, -(h.y + off), 0), glm::vec3(outer.x, t, outer.z));
	CreateWall(glm::vec3(0,  (h.y + off), 0), glm::vec3(outer.x, t, outer.z));
	CreateWall(glm::vec3( (h.x + off), 0, 0), glm::vec3(t, outer.y, outer.z));
	CreateWall(glm::vec3(-(h.x + off), 0, 0), glm::vec3(t, outer.y, outer.z));
	CreateWall(glm::vec3(0, 0, -(h.z + off)), glm::vec3(outer.x, outer.y, t));
	CreateWall(glm::vec3(0, 0,  (h.z + off)), glm::vec3(outer.x, outer.y, t));
}

void Collision3DScene::CreateWall(glm::vec3 position, glm::vec3 scale) {
	EmptyObject* wall = new EmptyObject("wall", nullptr, this);
	wall->SetPosition(position);
	wall->SetScale(scale);
	wall->AddComponent<BoxColliderComponent>();
}

void Collision3DScene::Update(double deltaTime) {
	if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Z) == GLFW_PRESS) {
		CreateSpheres();
	}
	else {
		spheresCreated = false;
	}
	ASceneController::Update(deltaTime);
}

void Collision3DScene::CreateSpheres() {
	if (spheresCreated) return;
	spheresCreated = true;

	for (int i = 0; i < SPHERE_COUNT; i++) {
		glm::vec3 randomPosition = RandomDirection() * SPAWN_RADIUS * (rand() / (float)RAND_MAX);
		glm::vec3 randomInitialVelocity = RandomDirection() * INITIAL_SPEED;

		EmptyObject* sphere = new EmptyObject("Sphere", nullptr, this);
		sphere->SetPosition(randomPosition);
		auto collider = sphere->AddComponent<SphereColliderComponent>();
		collider->SetRadius(SPHERE_RADIUS);
		auto rb = sphere->AddComponent<Rigidbody3DComponent>();
		rb->velocity = randomInitialVelocity;
		rb->gravityScale = 0;
	}
}
