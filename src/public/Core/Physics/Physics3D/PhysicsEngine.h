#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Physics/APhysicsEngine.h>

class AObject;

namespace Physics3D {
	/**
	 * This class encasulates all the physics calculations for 3D objects.
	 */
	class PhysicsEngine : public APhysicsEngine {

	public:
		/**
		 * This class represents a 3D raycast hit.
		 */
		class Raycast3D {
		public:
			bool hit = false;
			float time;
			glm::vec3 position;
		};
	};
}
