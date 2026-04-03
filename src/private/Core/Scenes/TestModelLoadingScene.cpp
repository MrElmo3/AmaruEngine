#include <Core/Scenes/TestModelLoadingScene.h>
#include <Core/Objects/EmptyObject.h>
#include <string>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/Light/DirectionalLight.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Components/Render/ModelRendererComponent.h>
#include <Core/Components/Render/RenderCubeComponent.h>
#include <Core/Objects/3D/Cube.h>
#include <Core/Render/Model.h>

TestModelLoadingScene::TestModelLoadingScene() {
	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(0, 0, 3);
	cameraObject->RotateEuler(glm::vec3(0, 0, 0));
	cameraComponent->mainCamera = true;

	DirectionalLight* light = new DirectionalLight("light", nullptr, this);

	testModel = new EmptyObject("testModel", nullptr, this);
	testModel->scale = glm::vec3(1);
	testModel->position = glm::vec3(0, -1, 0);
	ModelRendererComponent* modelRenderer = testModel->AddComponent<ModelRendererComponent>();
	
	auto* modelRendererMaterial  = new LitMaterial();
	// modelRendererMaterial->SetDiffuse("Assets/Textures/body1_Base_color.png");
	modelRenderer->material = modelRendererMaterial;

	modelRenderer->SetModel(new Model("Assets/Models/Reze.fbx"));
}

void TestModelLoadingScene::Update(double deltaTime) {
	ASceneController::Update(deltaTime);
	
	float rotationSpeed = 20.0f;
	testModel->RotateEuler(testModel->Up() * rotationSpeed * (float)deltaTime);

}