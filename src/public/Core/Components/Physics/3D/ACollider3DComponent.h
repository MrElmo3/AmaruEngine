#pragma once
#include "Core/Components/Physics/ACollider.h"
#include "Core/Components/Physics/3D/APhysics3DComponent.h"

class ACollider3DComponent : public ACollider, public APhysics3DComponent {
public:
	/**
	 * @brief Position of the center of the 3D collider (local to its object).
	 */
	glm::vec3 position;

	/**
	 * @brief World position of the center of the 3D collider.
	 */
	glm::vec3 worldPosition = glm::vec3(0);

	ACollider3DComponent(AObject* parent, glm::vec3 position);
	ACollider3DComponent(AObject* parent);
	virtual ~ACollider3DComponent() override;

	virtual void LateUpdate() override;
	virtual void MarkDirty() override;

private:
	/// @brief True when the world vertices are outdated with respect to the object transform.
	bool worldValuesDirty = true;

	void UpdateWorldVertexPoints() override;

protected:
	/// @brief Recomputes the world values (vertices, center, AABB) only if they are outdated.
	void EnsureWorldValues();

public:
	std::pair<glm::vec3, glm::vec3> GetAABBContainer() override;
	ACollider3DComponent* SetPosition(glm::vec3 position);
	glm::vec3 GetWorldPosition() { EnsureWorldValues(); return worldPosition; }
};
