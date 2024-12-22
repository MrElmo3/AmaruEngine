#include "Core/Scenes/TestScene.h"

#include "Core/Components/Physics/SquareColliderComponent.h"
#include "Core/Components/Render/RenderComponent.h"
#include "Core/Objects/SquareObject.h"
#include "Core/Objects/Player.h"
#include "Core/Materials/BaseMaterial.h"
#include "Core/Render/Render.h"

TestScene::TestScene() {
	// Player* square = new Player(nullptr, "Square");
	/*square->scale =Vector3(0.5, 0.5, 0.5);
	square->position = Vector3(-1, -1, 0);*/
	
	// objects.push_back(square);

	// SquareObject* square2 = new SquareObject(nullptr, "Square2");
	// square2->scale = glm::vec3(0.5, 0.5, 0.5);
	// square2->position = glm::vec3(0, 0, 0);
	// AddObject(square2);
	//
	// SquareObject* square1 = new SquareObject(nullptr, "Square2");
	// square1->scale = glm::vec3(0.5, 0.5, 0.5);
	// square1->position = glm::vec3(0, 1.0f, 0);
	// AddObject(square1);
	//
	// SquareObject* square3 = new SquareObject(nullptr, "Square2");
	// square3->scale = glm::vec3(0.5, 0.5, 0.5);
	// square3->position = glm::vec3(2.0f, 0, 0);
	// AddObject(square3);
}

void TestScene::Update(double deltaTime) {
	ASceneController::Update(deltaTime);
	Render::GetInstance().DrawText("Hello World", glm::vec3(25, 25, 0), 1.0f, glm::vec3(1.0f));
}