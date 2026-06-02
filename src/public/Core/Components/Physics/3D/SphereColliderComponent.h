#pragma once
#include <glm/glm.hpp>
#include <Core/Components/Physics/3D/ACollider3DComponent.h>

class SphereColliderComponent : public ACollider3DComponent {
private:
	static const int RENDER_LINE_NUMBER = 12;
	static const int RENDER_VERTEX_NUMBER = 12;

	float radius;

public:
	SphereColliderComponent(AObject* parent, glm::vec3 position, float radius);
	SphereColliderComponent(AObject* parent, glm::vec3 position);
	SphereColliderComponent(AObject* parent);
	~SphereColliderComponent() override;

protected:
	virtual void UpdateLocalVertexPoints() override;

public:
	glm::vec3 GetSupportPoint(glm::vec3 direction) override;
	float GetRadius() const { return radius; }

	SphereColliderComponent* SetRadius(float radius);
};
