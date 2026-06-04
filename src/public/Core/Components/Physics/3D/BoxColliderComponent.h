#pragma once
#include <glm/glm.hpp>
#include <Core/Components/Physics/3D/ACollider3DComponent.h>

class BoxColliderComponent : public ACollider3DComponent {
private:
	glm::vec3 halfSize;

public:
	BoxColliderComponent(AObject* parent, glm::vec3 position, glm::vec3 halfSize);
	BoxColliderComponent(AObject* parent, glm::vec3 position);
	BoxColliderComponent(AObject* parent);
	~BoxColliderComponent() override;

protected:
	virtual void UpdateLocalVertexPoints() override;

private:
	void UpdateWorldHalfSize();

public:
	glm::vec3 GetSupportPoint(glm::vec3 direction) override;

	BoxColliderComponent* SetHalfSize(glm::vec3 halfSize);
};
