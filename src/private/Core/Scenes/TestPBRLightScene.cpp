#include "Core/Scenes/TestPBRLightScene.h"
#include <Core/Objects/EmptyObject.h>
#include <string>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Components/Render/RenderCubeComponent.h>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/3D/Cube.h>
#include <Core/Objects/Light/DirectionalLight.h>
#include <Core/Objects/Light/PointLight.h>
#include <Core/Objects/Light/SpotLight.h>
#include <Core/Objects/General/CameraObject.h>
#include <Core/Materials/UnlitMaterial.h>
#include <Core/Render/Render.h>
#include <Core/Components/Render/ModelRendererComponent.h>
#include <Core/Render/Model.h>
// #include <Game/Components/MovementComponent.h>
// #include <Core/Components/Physics/3D/Rigidbody3DComponent.h>
// #include <Core/Components/Input/InputComponent.h>

TestPBRLightScene::TestPBRLightScene() {

	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(0, 0, 4);
	cameraObject->RotateEuler(glm::vec3(0, 0, 0));
	cameraComponent->mainCamera = true;

	// cameraObject->AddComponent(new Rigidbody3DComponent(cameraObject));
	// cameraObject->AddComponent(new InputComponent(cameraObject));
	
	// dynamic_cast<MovementComponent*>(cameraObject->AddComponent(new MovementComponent(cameraObject)))
	// 	->SetSpeed(2.0f)
	// 	->SetRotationSpeed(5.0f);
	
	testCube = new EmptyObject("testModel", nullptr, this);
	testCube->scale = glm::vec3(0.5);
	testCube->position = glm::vec3(0, 0, 0);
	testCube->RotateEuler(glm::vec3(0, 0, 0));

	ModelRendererComponent* modelRenderer = testCube->AddComponent<ModelRendererComponent>();
	
	auto* modelRendererMaterial  = new LitMaterial();
	modelRendererMaterial
		->SetAlbedo("Assets/Textures/1001_albedo.jpg")
		->SetNormal("Assets/Textures/1001_normal.png")
		->SetRoughness("Assets/Textures/1001_roughness.jpg")
		->SetAO("Assets/Textures/1001_AO.jpg")
		->IsPBR(true)
		->shader = Render::GetInstance().CreateShader(
			"Assets/Shaders/PBR/PBR.vert",
			"Assets/Shaders/PBR/PBR.frag"
		);

	modelRenderer->material = modelRendererMaterial;
	modelRenderer->SetModel(new Model("Assets/Models/Survival_BackPack_2.fbx"));

	DirectionalLight* light = new DirectionalLight("light", nullptr, this);
	
	PointLight* light2 = new PointLight("light2", nullptr, this);
	light2->position = glm::vec3(1, 0 , -1);
	light2->SetRange(10);

	// SpotLight* light3 = new SpotLight("light3", nullptr, this);
	// light3->position = glm::vec3(0, 0, 2);
	// light3->scale = glm::vec3(0.1);
	// light3->SetRange(200)
	// 	->SetSpotRange(5.5, 7.5);
}

void TestPBRLightScene::Update(double deltaTime) {
	ASceneController::Update(deltaTime);
	
	float rotationSpeed = 20.0f;
	testCube->RotateEuler(testCube->Up() * rotationSpeed * (float)deltaTime);

}
