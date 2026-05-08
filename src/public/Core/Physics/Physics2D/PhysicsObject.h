#pragma once
#include <vector>
#include <glm/glm.hpp>

class AObject;
class ACollider2DComponent;
class Rigidbody2DComponent;

namespace Physics2D 
{
	class PhysicObject {
	public:
		AObject* baseObject;
		Rigidbody2DComponent* rigidbody;
		std::vector<ACollider2DComponent*> colliders;

		glm::vec2 topRight;
		glm::vec2 bottomLeft;

		void UpdateLimits();
	};
}
