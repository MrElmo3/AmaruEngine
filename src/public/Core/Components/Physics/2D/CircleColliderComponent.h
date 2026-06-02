#pragma once
#include <glm/glm.hpp>
#include <Core/Components/Physics/2D/ACollider2DComponent.h>

class CircleColliderComponent : public ACollider2DComponent{
private:
	
	static const int RENDER_VERTEX_NUMBER = 12;

	float radius;

public:
	CircleColliderComponent(AObject* parent, glm::vec2 position, float radius);
	CircleColliderComponent(AObject* parent, glm::vec2 position);
	CircleColliderComponent(AObject* parent);
	~CircleColliderComponent() override;

protected:
	virtual void UpdateLocalVertexPoints() override;

public:
	glm::vec3 GetSupportPoint(glm::vec3 direction) override;
	float GetRadius() const { return radius; }

	CircleColliderComponent* SetRadius(float radius);
};
