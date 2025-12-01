#include <Core/Scenes/TestModelLoadingScene.h>
#include <Core/Objects/EmptyObject.h>
#include <string>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/Light/DirectionalLight.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/ModelRendererComponent.h>
#include <Core/Render/Model.h>

TestModelLoadingScene::TestModelLoadingScene() {
	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(4, 0, 4);
	cameraObject->RotateEuler(glm::vec3(0, -45, 0));
	cameraComponent->mainCamera = true;

	DirectionalLight* light = new DirectionalLight("light", nullptr, this);

	testModel = new EmptyObject("testModel", nullptr, this);
	testModel->scale = glm::vec3(0.5);
	testModel->position = glm::vec3(0);
	ModelRendererComponent* modelRenderer = testModel->AddComponent<ModelRendererComponent>();
	
	auto* modelRendererMaterial  = new LitMaterial();
	modelRendererMaterial->SetDiffuse("Assets/Textures/1001_albedo.jpg");
	modelRenderer->material = modelRendererMaterial;

	modelRenderer->SetModel(new Model("Assets/Models/Survival_BackPack_2.fbx"));
}

void TestModelLoadingScene::Update(double deltaTime) {
	ASceneController::Update(deltaTime);
	
	float rotationSpeed = 20.0f;
	testModel->RotateEuler(testModel->Right() * rotationSpeed * (float)deltaTime);

}