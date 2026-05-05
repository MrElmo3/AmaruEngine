#include <Core/Scenes/TestScene.h>

#include <Core/Materials/LitMaterial.h>

#include <Core/Components/Physics/2D/SquareColliderComponent.h>
#include <Core/Components/Render/CameraComponent.h>
#include <Core/Components/Render/RenderQuadComponent.h>
#include <Core/Objects/2D/Square.h>
#include <Game/Objects/Player.h>
#include <Core/Objects/3D/Cube.h>
#include <Core/Objects/EmptyObject.h>
#include <Core/Render/Render.h>

TestScene::TestScene() {
	// Player* square = new Player(nullptr, "Square");
	/*square->scale =Vector3(0.5, 0.5, 0.5);
	square->SetPosition(Vector3(-1, -1, 0);*/
	
	// objects.push_back(square);

	auto* square2 = new Square("Square2", nullptr, this);
	square2->SetScale(glm::vec3(1, 1, 1));
	square2->SetPosition(glm::vec3(0, 0, 0));
	dynamic_cast<LitMaterial*>(square2->GetRenderComponent()->material);
		// ->SetColor(glm::vec3(1, 0.5, 1));
		// ->SetTexture("Assets/Textures/HL.jpg");
	
	// AddObject(square2);

	auto* cameraObject = new EmptyObject("Camera", nullptr, this);
	auto* cameraComponent = cameraObject->AddComponent<CameraComponent>();
	cameraObject->SetPosition(glm::vec3(0, 0, 10));
	cameraComponent->orthographic = true;
	cameraComponent->mainCamera = true;
	cameraComponent->orthoSize = 2.0f;
	// AddObject(cameraObject);

	// CubeObject* cube = new CubeObject(nullptr, "Cube");
	// cube->SetScale(glm::vec3(1, 1, 1);
	// cube->SetPosition(glm::vec3(2, 2, 0);
	// AddObject(cube);
	
	// SquareObject* square1 = new SquareObject(nullptr, "Square2");
	// square1->SetScale(glm::vec3(0.5, 0.5, 0.5);
	// square1->SetPosition(glm::vec3(0, 1.0f, 0);
	// AddObject(square1);
	//
	// SquareObject* square3 = new SquareObject(nullptr, "Square2");
	// square3->SetScale(glm::vec3(0.5, 0.5, 0.5);
	// square3->SetPosition(glm::vec3(2.0f, 0, 0);
	// AddObject(square3);
}
