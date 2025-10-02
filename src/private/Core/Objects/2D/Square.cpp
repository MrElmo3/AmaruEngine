#include <Core/Objects/2D/Square.h>
#include <Core/Components/Render/RenderQuadComponent.h>
#include <Core/Components/Physics/2D/SquareColliderComponent.h>

Square::Square(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene){

	AddComponent<RenderQuadComponent>();
	AddComponent<SquareColliderComponent>()
		->SetCenter(glm::vec2(0.f))
		->SetHalfSize(glm::vec2(0.5f));
}
