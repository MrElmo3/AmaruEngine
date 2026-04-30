#pragma once
#include "Core/Components/IComponent.h"
#include <glm/glm.hpp>

class AColliderComponent : public IComponent {
public:
	bool isTrigger = false;

	AColliderComponent(AObject* parent)
		: IComponent(parent) {}

	glm::vec3 GetSupportPoint(glm::vec3 direction);
	
protected:
	void DrawDebugOutline();
};
