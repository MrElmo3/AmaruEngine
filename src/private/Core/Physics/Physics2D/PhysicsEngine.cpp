#include <Core/Physics/Physics2D/PhysicsEngine.h>
#include <Core/Physics/Physics2D/PhysicsObject.h>
#include <Core/Physics/Physics2D/Quadtree.h>
#include <Core/Global.h>
#include <Core/Components/Physics/2D/ACollider2DComponent.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>
#include <Core/Objects/AObject.h>

namespace Physics2D {

	PhysicsEngine::PhysicsEngine() {
		quadtree = new Quadtree();
	}

	void PhysicsEngine::RegisterObject(AObject* object){
		Rigidbody2DComponent* rigidbody = object->GetComponent<Rigidbody2DComponent>();
		
		if(rigidbody != nullptr && rigidbody->PhysicsEnabled()){
			PhysicObject* newPhysicObject = new PhysicObject();
			newPhysicObject->baseObject = object;
			newPhysicObject->rigidbody = rigidbody;
			newPhysicObject->colliders = object->GetAllComponentsInChildren<ACollider2DComponent>();
			newPhysicObject->UpdateLimits();
			physicObjects.push_back(newPhysicObject);
			return;
		}

		auto colliders = object->GetComponentsOfType<ACollider2DComponent>();
		if(!colliders.empty()){
			PhysicObject* newPhysicObject = new PhysicObject();
			newPhysicObject->baseObject = object;
			newPhysicObject->rigidbody = nullptr;
			newPhysicObject->colliders = colliders;
			newPhysicObject->UpdateLimits();
			physicObjects.push_back(newPhysicObject);
		}

		for (auto child : object->GetChildren()) {
			RegisterObject(child);
		}
	}

	void PhysicsEngine::MoveObjects() {
		for(auto physicObject : physicObjects){
			if(physicObject->rigidbody != nullptr)
				MoveRigidbody(physicObject->rigidbody);
		}
	}

	void PhysicsEngine::MoveRigidbody(Rigidbody2DComponent* rigidbody) {
		UpdateAceleration(rigidbody);
		UpdateVelocity(rigidbody);
		UpdatePosition(rigidbody);
	}

	void PhysicsEngine::UpdateAceleration(Rigidbody2DComponent* rigidbody) {
		rigidbody->forceAccumulator += glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y) * rigidbody->gravityScale ;
		rigidbody->aceleration = rigidbody->forceAccumulator / rigidbody->mass;
		rigidbody->forceAccumulator = glm::vec3(0);
	}

	void PhysicsEngine::UpdateVelocity(Rigidbody2DComponent* rigidbody) {
		rigidbody->velocity += rigidbody->aceleration * Global::FIXED_DELTA_TIME;
	}

	void PhysicsEngine::UpdatePosition(Rigidbody2DComponent* rigidbody) {
		rigidbody->parent->Translate(glm::vec3(rigidbody->velocity * Global::FIXED_DELTA_TIME, 0.f));
	}

	void PhysicsEngine::UpdateTree() {
		UpdateContainers();
		quadtree->Build(physicObjects, worldMin, worldMax);
	}

	void PhysicsEngine::UpdateContainers() {
		for (auto physicObject : physicObjects) {
			physicObject->UpdateLimits();
		}
	}

	void PhysicsEngine::CheckCollisions() {
			//	Check collisions
		//		GetNearColliders
		//		Check if the RB Colliders colied with one of the near colliders
		//		If so, ResolveColision
		//			Calculates Manifold
		//			Updates RB's velocity
		//			Repell RB
	}
}