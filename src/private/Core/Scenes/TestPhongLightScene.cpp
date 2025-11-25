#include "Core/Scenes/TestPhongLightScene.h"
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
// #include <Game/Components/MovementComponent.h>
// #include <Core/Components/Physics/3D/Rigidbody3DComponent.h>
// #include <Core/Components/Input/InputComponent.h>

TestPhongLightScene::TestPhongLightScene() {

	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(4, 0, 4);
	cameraObject->RotateEuler(glm::vec3(0, -45, 0));
	cameraComponent->mainCamera = true;

	// cameraObject->AddComponent(new Rigidbody3DComponent(cameraObject));
	// cameraObject->AddComponent(new InputComponent(cameraObject));
	
	// dynamic_cast<MovementComponent*>(cameraObject->AddComponent(new MovementComponent(cameraObject)))
	// 	->SetSpeed(2.0f)
	// 	->SetRotationSpeed(5.0f);
	
	testCube = new Cube( "Square2", nullptr, this);
	testCube->scale = glm::vec3(1, 1, 1);
	testCube->position = glm::vec3(0, 0, 0);
	dynamic_cast<LitMaterial*>(testCube->GetRenderComponent()->material)
		->SetColor(1.0f, 0.0f, 0.0f, 1.0f)
		->SetDiffuse("Assets/Textures/container2.png")
		->SetSpecular("Assets/Textures/container2_specular.png");

	DirectionalLight* light = new DirectionalLight("light", nullptr, this);
	
	PointLight* light2 = new PointLight("light2", nullptr, this);
	light2->position = glm::vec3(2, 0 , -2);
	light2->SetRange(25);

	SpotLight* light3 = new SpotLight("light3", nullptr, this);
	light3->position = glm::vec3(0, 0, 2);
	light3->scale = glm::vec3(0.1);
	light3->SetRange(200)
		->SetSpotRange(12.5, 17.5);
}

void TestPhongLightScene::Update(double deltaTime) {
	ASceneController::Update(deltaTime);
	
	float rotationSpeed = 20.0f;
	testCube->RotateEuler(testCube->Right() * rotationSpeed * (float)deltaTime);

}
