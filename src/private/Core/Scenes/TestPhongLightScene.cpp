#include "Core/Scenes/TestPhongLightScene.h"
#include <Core/Objects/EmptyObject.h>
#include <string>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Components/Render/RenderCubeComponent.h>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/3D/Cube.h>
#include <Core/Objects/Light/PointLight.h>
#include <Core/Objects/General/CameraObject.h>

TestPhongLightScene::TestPhongLightScene() {

	auto* cameraObject = new CameraObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->GetCameraComponent();
	cameraObject->position = glm::vec3(4, 0, 4);
	cameraObject->RotateEuler(glm::vec3(0, -45, 0));
	cameraComponent->mainCamera = true;

	auto* cube = new Cube( "Square2", nullptr, this);
	cube->scale = glm::vec3(1, 2, 1);
	cube->position = glm::vec3(-0.5f, 0, 0);
	dynamic_cast<LitMaterial*>(cube->GetRenderComponent()->material)
		//->SetColor(1.0f, 0.0f, 0.0f, 1.0f);
		->SetTexture("Assets/Textures/HL.jpg");

	auto* lightSource = new PointLight("light", nullptr, this);
	lightSource->scale = glm::vec3(0.1f, 0.1f, 0.1f);
	lightSource->position = glm::vec3(1, 1, 1);
	lightSource->SetColor(1.0, 1.0, 1.0);
	lightSource->SetIntensity(1.0f);
	lightSource->SetRange(5.0f);
	lightSource->AddComponent(new RenderCubeComponent(lightSource));

}
