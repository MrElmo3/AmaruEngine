#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Physics/APhysicsEngine.h>

class AObject;
class ACollider2DComponent;
class Rigidbody2DComponent;

namespace Physics2D {
	class PhysicObject;
	class Quadtree;

	/**
	 * This class encasulates all the physics calculations for 2D objects.
	 */
	class PhysicsEngine : public APhysicsEngine {

	private:
		std::vector<PhysicObject*> physicObjects;

		Quadtree* quadtree;
		glm::vec2 worldMin = 	{ -500, -500 };
		glm::vec2  worldMax =  	{ 500, 500};
	
	public:
		PhysicsEngine();

		
	protected:
		virtual void RegisterObject(AObject* object) override;

		virtual void MoveObjects() override;
		void MoveRigidbody(Rigidbody2DComponent* rigidbody);
		void UpdateAceleration(Rigidbody2DComponent* rigidbody);
		void UpdateVelocity(Rigidbody2DComponent* rigidbody);
		void UpdatePosition(Rigidbody2DComponent* rigidbody);

		virtual void UpdateTree() override;
		void UpdateContainers();

		virtual void CheckCollisions() override;
	};
}
