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
// #include <Game/Components/MovementComponent.h>
// #include <Core/Components/Physics/3D/Rigidbody3DComponent.h>
// #include <Core/Components/Input/InputComponent.h>

TestPBRLightScene::TestPBRLightScene() {

	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(1.5, 0, 1.5);
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
		->SetAlbedo("Assets/Textures/StoneWall/albedo.png")
		->SetNormal("Assets/Textures/StoneWall/normal.png")
		->SetRoughness("Assets/Textures/StoneWall/roughness.png")
		->SetAO("Assets/Textures/StoneWall/ao.png")
		->IsPBR(true)
		->shader = Render::GetInstance().CreateShader(
			"Assets/Shaders/PBR/PBR.vert",
			"Assets/Shaders/PBR/PBR.frag"
		);

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
	
	float rotationSpeed = 0.0f;
	testCube->RotateEuler(testCube->Up() * rotationSpeed * (float)deltaTime);

}
