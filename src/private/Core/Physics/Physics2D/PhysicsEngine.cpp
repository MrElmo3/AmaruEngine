#include <Core/Components/Physics/2D/ACollider2DComponent.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>
#include <Core/Global.h>
#include <Core/Objects/AObject.h>
#include <Core/Physics/Physics2D/PhysicsEngine.h>
#include <Core/Physics/Physics2D/PhysicsObject.h>
#include <Core/Physics/Physics2D/Quadtree.h>
#include <Core/Render/Color.h>
#include <Util/Logger.h>

namespace Physics2D {

PhysicsEngine::PhysicsEngine() {
	quadtree = new Quadtree();
}

void PhysicsEngine::RegisterObject(AObject* object) {
	Rigidbody2DComponent* rigidbody = object->GetComponent<Rigidbody2DComponent>();

	if (rigidbody != nullptr && rigidbody->PhysicsEnabled()) {
		PhysicObject* newPhysicObject = new PhysicObject();
		newPhysicObject->baseObject = object;
		newPhysicObject->rigidbody = rigidbody;
		newPhysicObject->colliders = object->GetAllComponentsInChildren<ACollider2DComponent>();
		newPhysicObject->UpdateLimits();
		physicObjects.push_back(newPhysicObject);
		return;
	}

	auto colliders = object->GetComponentsOfType<ACollider2DComponent>();
	if (!colliders.empty()) {
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
	for (auto physicObject : physicObjects) {
		if (physicObject->rigidbody != nullptr)
			MoveRigidbody(physicObject->rigidbody);
	}
}

void PhysicsEngine::MoveRigidbody(Rigidbody2DComponent* rigidbody) {
	UpdateAceleration(rigidbody);
	UpdateVelocity(rigidbody);
	UpdatePosition(rigidbody);
}

void PhysicsEngine::UpdateAceleration(Rigidbody2DComponent* rigidbody) {
	rigidbody->forceAccumulator += glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y) * rigidbody->gravityScale;
	rigidbody->acceleration = rigidbody->forceAccumulator * rigidbody->GetInverseMass();
	rigidbody->forceAccumulator = glm::vec3(0);
}

void PhysicsEngine::UpdateVelocity(Rigidbody2DComponent* rigidbody) {
	float dt = Global::FIXED_DELTA_TIME;

	rigidbody->velocity += rigidbody->acceleration * dt;
	rigidbody->angularVelocity += rigidbody->angularAcceleration * dt;
	
	// rigidbody->velocity *= 0.99f;
	// rigidbody->angularVelocity *= 0.95f;

	const float sleepThresholdLinear = 0.05f;
	const float sleepThresholdAngular = 0.29f; // Radians per second

	if (glm::length(rigidbody->velocity) < sleepThresholdLinear) {
		rigidbody->velocity = glm::vec2(0.0f);
	}

	if (abs(rigidbody->angularVelocity) < sleepThresholdAngular) {
		rigidbody->angularVelocity = 0.0f;
	}
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
	for (auto object : physicObjects) {
		if (object->rigidbody == nullptr)
			continue;
		std::vector<PhysicObject*> nearObjects;
		quadtree->GetNearObjects(object, nearObjects);
		for (auto nearObject : nearObjects) {
			CheckCollision(object, nearObject);
		}
	}
}

bool PhysicsEngine::CheckCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB) {
	for (auto colliderA : physicObjectA->colliders) {
		for (auto colliderB : physicObjectB->colliders) {
			PhysicsEngine::GJKResult result = GJKCheck(colliderA, colliderB);

			if (result.Collide) {
				PhysicsEngine::CollisionManifold manifold = EPA(colliderA, colliderB, result.Simplex);

				ResolveCollision2D(physicObjectA, physicObjectB, manifold);

				colliderA->debugColor = Color::RED;
				colliderB->debugColor = Color::RED;
				return true;
			}
		}
	}
	return false;
}

void PhysicsEngine::ResolveCollision2D(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold) {
	if (physicObjectA->rigidbody != nullptr && physicObjectB->rigidbody != nullptr) {
		Resolve2RBCollision(physicObjectA, physicObjectB, manifold);
		return;
	}

	if (physicObjectA->rigidbody != nullptr) {
		Resolve1RBCollision(physicObjectA, manifold);
		return;
	}

	if (physicObjectB->rigidbody != nullptr) {
		Resolve1RBCollision(physicObjectB, manifold);
		return;
	}
};

void PhysicsEngine::Resolve1RBCollision(PhysicObject* physicObject, CollisionManifold manifold) {
	auto baseObject = physicObject->baseObject;
	auto rb = physicObject->rigidbody;

	glm::vec2 normal = manifold.PenetrationVector;

	const float slop = 0.01f;
    const float percent = 0.8f;

	if (manifold.Depth > slop) {
        float correction = (manifold.Depth - slop) * percent;
        baseObject->Translate(glm::vec3(-correction * normal, 0.0f));
    }

    glm::vec2 contactPoint = glm::vec2(manifold.ContactPoint);

    glm::vec2 contactVelocity = rb->velocity;

	float normalVelocity = glm::dot(contactVelocity, normal);

	if(normalVelocity < 0)  return;
	
	float restitution = 0.9f; // TODO: Pull from physical properties

	float gravityStep = glm::length(glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y)) * Global::FIXED_DELTA_TIME;
	if (normalVelocity < (gravityStep * 2.0f)) {
		restitution = 0.0f;
	}
	float impulseMagnitude = (1.0f + restitution) * normalVelocity;

	glm::vec2 linearImpulse = impulseMagnitude * normal;
	rb->velocity -= linearImpulse;
	

};

void PhysicsEngine::Resolve2RBCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold) {
}

}