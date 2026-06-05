#pragma once
#include <glm/glm.hpp>
#include <Core/Components/Physics/2D/ACollider2DComponent.h>

class SquareColliderComponent : public ACollider2DComponent{
private:
	glm::vec2 halfSize;

public:
	SquareColliderComponent(AObject* parent, glm::vec2 position, glm::vec2 halfSize);
	SquareColliderComponent(AObject* parent, glm::vec2 position);
	SquareColliderComponent(AObject* parent);
	~SquareColliderComponent() override;

protected:
	virtual void UpdateLocalVertexPoints() override;

public:
	glm::vec3 GetSupportPoint(glm::vec3 direction) override;

	SquareColliderComponent* SetHalfSize(glm::vec2 halfSize);
};
