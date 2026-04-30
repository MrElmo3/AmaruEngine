#pragma once
#include <glm/glm.hpp>
#include <Core/Objects/AObject.h>
#include "ACollider3DComponent.h"

class BoxColliderComponent {
// private:
// 	glm::vec3 center;
// 	glm::vec3 halfSize;

	glm::vec3 worldCenter;
	glm::vec3 worldHalfSize;

public:
	BoxColliderComponent(AObject* parent);
	BoxColliderComponent(AObject* parent, glm::vec3 center, glm::vec3 halfSize);
	~BoxColliderComponent();

	void FixedUpdate();
	void LateUpdate();

	glm::vec3 GetWorldCenter() const { return worldCenter; }
	glm::vec3 GetWorldHalfSize() const { return worldHalfSize; }

	BoxColliderComponent* SetCenter(glm::vec3 center);
	BoxColliderComponent* SetHalfSize(glm::vec3 halfSize);
};
