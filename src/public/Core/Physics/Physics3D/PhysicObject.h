#pragma once
#include <vector>
#include <glm/glm.hpp>

class AObject;
class ACollider3DComponent;
class Rigidbody3DComponent;

namespace Physics3D 
{
	class PhysicObject {
	public:
		AObject* baseObject;
		Rigidbody3DComponent* rigidbody;
		std::vector<ACollider3DComponent*> colliders;

		glm::vec3 topRight;
		glm::vec3 bottomLeft;

		void UpdateLimits();
	};
}
