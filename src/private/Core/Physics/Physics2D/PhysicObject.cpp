#include <Core/Physics/Physics2D/PhysicObject.h>

#include <Core/Components/Physics/2D/ACollider2DComponent.h>

namespace Physics2D {

void PhysicObject::UpdateLimits() {
	bottomLeft = glm::vec2(FLT_MAX);
	topRight = glm::vec2(-FLT_MAX);

	if (colliders.empty())
		return;

	for (auto collider : colliders) {
		auto [cMin, cMax] = collider->GetAABBContainer();
		bottomLeft = glm::vec2(std::min(bottomLeft.x, cMin.x), std::min(bottomLeft.y, cMin.y));
		topRight = glm::vec2(std::max(topRight.x, cMax.x), std::max(topRight.y, cMax.y));
	}
}
}