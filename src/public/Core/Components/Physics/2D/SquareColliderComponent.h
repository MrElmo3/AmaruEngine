#pragma once
#include <glm/glm.hpp>
#include <Core/Components/Physics/2D/ACollider2DComponent.h>

class SquareColliderComponent : public ACollider2DComponent{
private:
	glm::vec2 halfSize;
	glm::vec2 worldHalfSize;

public:
	SquareColliderComponent(AObject* parent, glm::vec2 position, glm::vec2 halfSize);
	SquareColliderComponent(AObject* parent, glm::vec2 position);
	SquareColliderComponent(AObject* parent);
	~SquareColliderComponent() override;

	virtual void FixedUpdate() override;
	virtual void LateUpdate() override;

protected:
	virtual void UpdateVertexPoints() override;

private:
	void UpdateWorldHalfSize();

public:
	glm::vec3 GetSupportPoint(glm::vec2 direction);
	glm::vec3 GetSupportPoint(glm::vec3 direction) override;
	glm::vec2 GetWorldHalfSize() const { return worldHalfSize; }

	SquareColliderComponent* SetHalfSize(glm::vec2 halfSize);
};
