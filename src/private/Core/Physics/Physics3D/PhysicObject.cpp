#include <Core/Physics/Physics3D/PhysicObject.h>

#include <Core/Components/Physics/3D/ACollider3DComponent.h>

namespace Physics3D {

void PhysicObject::UpdateLimits() {
	bottomLeft = glm::vec3(FLT_MAX);
	topRight = glm::vec3(-FLT_MAX);

	if (colliders.empty())
		return;

	for (auto collider : colliders) {
		auto [cMin, cMax] = collider->GetAABBContainer();
		bottomLeft = glm::vec3(
			std::min(bottomLeft.x, cMin.x), 
			std::min(bottomLeft.y, cMin.y), 
			std::min(bottomLeft.z, cMin.z)
		);
		topRight = glm::vec3(
			std::max(topRight.x, cMax.x), 
			std::max(topRight.y, cMax.y),
			std::max(topRight.z, cMax.z)
		);
	}
}
}