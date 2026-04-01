#include <Core/Scenes/TestModelLoadingScene.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Objects/EmptyObject.h>
#include <Core/Components/Render/ModelRendererComponent.h>
#include <Core/Objects/Light/DirectionalLight.h>
#include <Core/Objects/3D/Cube.h>

TestModelLoadingScene::TestModelLoadingScene() {
	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(4, 0, 4);
	cameraObject->RotateEuler(glm::vec3(0, -45, 0));
	cameraComponent->mainCamera = true;

	DirectionalLight* light = new DirectionalLight("light", nullptr, this);
	
	// PointLight* light2 = new PointLight("light2", nullptr, this);
	// light2->position = glm::vec3(2, 0 , -2);
	// light2->SetRange(25);

	// SpotLight* light3 = new SpotLight("light3", nullptr, this);
	// light3->position = glm::vec3(0, 0, 2);
	// light3->scale = glm::vec3(0.1);
	// light3->SetRange(200)
	// 	->SetSpotRange(12.5, 17.5);

	// auto* testCube = new Cube("Square2", nullptr, this);

	auto* emptyObject = new EmptyObject("Model", nullptr, this);
	auto* modelRenderer = emptyObject->AddComponent<ModelRendererComponent>()
		->SetModel("Assets/Models/Survival_BackPack_2.fbx");
}