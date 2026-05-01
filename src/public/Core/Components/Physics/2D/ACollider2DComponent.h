#pragma once
#include "Core/Components/Physics/ACollider.h"
#include "Core/Components/Physics/2D/APhysics2DComponent.h"

class ACollider2DComponent : public ACollider, public APhysics2DComponent {
public:
	/**
	 * @brief Position of the center of the 2D collider
	 */
	glm::vec2 position;

	glm::vec2 worldPosition;

	ACollider2DComponent(AObject* parent, glm::vec2 position);
	ACollider2DComponent(AObject* parent);
	virtual ~ACollider2DComponent() override;

	virtual void FixedUpdate() override;

private:
	void UpdateWorldPosition();
	
public:
	ACollider2DComponent* SetPosition(glm::vec2 position);
	glm::vec2 GetWorldPosition() const { return worldPosition; }
};
