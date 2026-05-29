#pragma once
#include "Core/Components/Physics/ACollider.h"
#include "Core/Components/Physics/3D/APhysics3DComponent.h"

class ACollider3DComponent : public ACollider, public APhysics3DComponent {
public:
	/**
	 * @brief Position of the center of the 2D collider
	 */
	glm::vec3 position;

	/**
	 * @brief world position of the center of the 2D collider
	 */
	glm::vec3 worldPosition;

	ACollider3DComponent(AObject* parent, glm::vec3 position);
	ACollider3DComponent(AObject* parent);
	virtual ~ACollider3DComponent() override;

	virtual void LateUpdate() override;

private:
	void UpdateWorldPosition();
	void UpdateWorldVertexPoints() override;
	
public:
	std::pair<glm::vec3, glm::vec3> GetAABBContainer() override;
	glm::vec3 GetSupportPoint(glm::vec3 direction);
	ACollider3DComponent* SetPosition(glm::vec3 position);
	glm::vec2 GetWorldPosition() const { return worldPosition; }
};
