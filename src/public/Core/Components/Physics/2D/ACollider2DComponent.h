#pragma once
#include "Core/Components/Physics/ACollider.h"
#include "Core/Components/Physics/2D/APhysics2DComponent.h"

class ACollider2DComponent : public ACollider, public APhysics2DComponent {
public:
	ACollider2DComponent(AObject* parent);
};
