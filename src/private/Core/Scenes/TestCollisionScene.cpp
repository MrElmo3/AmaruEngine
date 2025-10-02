#include <Core/Scenes/TestCollisionScene.h>

#include <Core/Global.h>
#include <Core/Components/Render/RenderQuadComponent.h>
#include <Core/Components/Input/FollowMouseComponent.h>
#include <Core/Components/Input/InputComponent.h>
#include <Game/Components/MovementComponent.h>
#include <Core/Components/Physics/2D/SquareColliderComponent.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>
#include <Core/Objects/2D/Square.h>
#include <Core/Objects/3D/Cube.h>
#include <Core/Render/Color.h>
#include <Core/Render/Render.h>

TestColisionScene::TestColisionScene() {
	
	auto* testSquare = new Square("squareMouse", nullptr, this);
	testSquare->GetRenderComponent()->enableRender = true;
	// testSquare->AddComponent(new FollowMouseComponent(testSquare));
	testSquare->AddComponent<Rigidbody2DComponent>();
	testSquare->AddComponent<InputComponent>();
	MovementComponent* movement =  testSquare->AddComponent<MovementComponent>();
	testSquare->position = glm::vec3(-0.5, 0.5, 0);
	testSquare->scale = glm::vec3(0.3f);
	movement->SetSpeed(1);
	// movement->SetDirection(glm::vec2(1, -1));
	AddObject(testSquare);

	auto* staticSquare = new Square("staticSquare", nullptr, this);
	staticSquare->GetRenderComponent()->enableRender = false;
	// staticSquare->GetColliderComponent()->isTrigger = true;
	// staticSquare->AddComponent(new FollowMouseComponent(staticSquare));
	staticSquare->scale = glm::vec3(0.5f);
	AddObject(staticSquare);

	auto* sumSquare = new Square("sumSquare", nullptr, this );
	sumSquare->GetRenderComponent()->enableRender = false;
	sumSquare->GetColliderComponent()->isTrigger = true;
	sumSquare->position = glm::vec3(staticSquare->GetWorldPosition());
	sumSquare->scale = staticSquare->scale + testSquare->scale ;
	AddObject(sumSquare);

	// SquareObject* resultSquare = new SquareObject(nullptr, "resultSquare");
	// resultSquare->GetRenderComponent()->enableRender = false;
	// resultSquare->scale = Vector3(0.3);
	// AddObject(resultSquare);

	// SquareObject* startSquare = new SquareObject(nullptr, "startSquare");
	// startSquare->GetRenderComponent()->enableRender = false;
	// startSquare->position = Vector2(1.5, 1.7);
	// startSquare->scale = Vector3(0.3);
	// AddObject(startSquare);
	
}

void TestColisionScene::Update(double deltaTime) {
	// std::cout<<deltaTime<<'\n';

	auto* testSquare = dynamic_cast<Square*>(objects[0]);
	Rigidbody2DComponent* rbTestSquare = nullptr;
	for (IComponent* component : testSquare->components) {
		rbTestSquare = dynamic_cast<Rigidbody2DComponent*>(component);
		if (rbTestSquare != nullptr) {
			break;
		}
	}
	
	// Render::GetInstance().DrawLineSegment(
	// 	testSquare->GetGlobalPosition(),
	// 	testSquare->GetGlobalPosition() + rbTestSquare->velocity * Global::FIXED_DELTA_TIME,
	// 	Color::RED
	// 	);
	
	ASceneController::Update(deltaTime);

}