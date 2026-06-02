#include <Core/Physics/Physics3D/PhysicsEngine.h>
#include <Core/Physics/Physics3D/PhysicObject.h>
#include <Core/Physics/Physics3D/Octree.h>
#include <Core/Objects/AObject.h>
#include <Core/Components/Physics/3D/ACollider3DComponent.h>
#include <Core/Components/Physics/3D/Rigidbody3DComponent.h>
#include <Core/Render/Color.h>

namespace Physics3D {

PhysicsEngine::PhysicsEngine() {
	octree = new Octree();
}

void PhysicsEngine::RegisterObject(AObject* object) {
	Rigidbody3DComponent* rigidbody = object->GetComponent<Rigidbody3DComponent>();

	if (rigidbody != nullptr && rigidbody->PhysicsEnabled()) {
		PhysicObject* newPhysicObject = new PhysicObject();
		newPhysicObject->baseObject = object;
		newPhysicObject->rigidbody = rigidbody;
		newPhysicObject->colliders = object->GetAllComponentsInChildren<ACollider3DComponent>();
		newPhysicObject->UpdateLimits();
		physicObjects.push_back(newPhysicObject);
		return;
	}

	auto colliders = object->GetComponentsOfType<ACollider3DComponent>();
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

void PhysicsEngine::MoveRigidbody(Rigidbody3DComponent* rigidbody) {
	UpdateAceleration(rigidbody);
	UpdateVelocity(rigidbody);
	UpdatePosition(rigidbody);
}

void PhysicsEngine::UpdateAceleration(Rigidbody3DComponent* rigidbody) {
	rigidbody->forceAccumulator += Global::GRAVITY * rigidbody->gravityScale;
	rigidbody->acceleration = rigidbody->forceAccumulator * rigidbody->GetInverseMass();
	rigidbody->forceAccumulator = glm::vec3(0);
}

void PhysicsEngine::UpdateVelocity(Rigidbody3DComponent* rigidbody) {
	float dt = Global::FIXED_DELTA_TIME;

	rigidbody->velocity += rigidbody->acceleration * dt;
	// rigidbody->angularVelocity += rigidbody->angularAcceleration * dt;
	
	// rigidbody->velocity *= 0.99f;
	// rigidbody->angularVelocity *= 0.95f;

	const float sleepThresholdLinear = 0.05f;
	const float sleepThresholdAngular = 0.29f; // Radians per second

	if (glm::length(rigidbody->velocity) < sleepThresholdLinear) {
		rigidbody->velocity = glm::vec3(0.0f);
	}
}

void PhysicsEngine::UpdatePosition(Rigidbody3DComponent* rigidbody) {
	rigidbody->parent->Translate(glm::vec3(rigidbody->velocity * Global::FIXED_DELTA_TIME));
}

void PhysicsEngine::UpdateTree() {
	UpdateContainers();
	octree->Build(physicObjects, worldMin, worldMax);
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
		octree->GetNearObjects(object, nearObjects);
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

	glm::vec3 normal = manifold.PenetrationVector;

	const float slop = 0.01f;
    const float percent = 0.8f;

	if (manifold.Depth > slop) {
        float correction = (manifold.Depth - slop) * percent;
        baseObject->Translate(-correction * normal);
    }

    glm::vec3 contactVelocity = rb->velocity;

	float normalVelocity = glm::dot(contactVelocity, normal);

	if(normalVelocity < 0)  return;
	
	float restitution = 0.9f; // TODO: Pull from physical properties

	float gravityStep = glm::length(glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y)) * Global::FIXED_DELTA_TIME;
	if (normalVelocity < (gravityStep * 2.0f)) {
		restitution = 0.0f;
	}
	float impulseMagnitude = (1.0f + restitution) * normalVelocity;

	glm::vec3 linearImpulse = impulseMagnitude * normal;
	rb->velocity -= linearImpulse;
	
};

void PhysicsEngine::Resolve2RBCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB, CollisionManifold manifold) {
	auto rbA = physicObjectA->rigidbody;
    auto rbB = physicObjectB->rigidbody;

    float invMassA = rbA->GetInverseMass();
    float invMassB = rbB->GetInverseMass();
    float totalInvMass = invMassA + invMassB;

	if (totalInvMass <= 0.0f)
		return;
	
	glm::vec3 normal = manifold.PenetrationVector;

	const float slop = 0.01f;
    const float percent = 0.8f; 
    
    if (manifold.Depth > slop) {
        float correctionAmount = (manifold.Depth - slop) * percent;
        
        glm::vec3 correctionVector = normal * (correctionAmount / totalInvMass);

        physicObjectA->baseObject->Translate(-correctionVector * invMassA);
        physicObjectB->baseObject->Translate(correctionVector * invMassB);
    }

	glm::vec3 relativeVelocity = rbB->velocity - rbA->velocity;
    float normalVelocity = glm::dot(relativeVelocity, normal);

	if (normalVelocity < 0.0f) {
		float restitution = 0.9f; //TODO: restitution
		
		float gravityStep = glm::length(glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y)) * Global::FIXED_DELTA_TIME;
        if (abs(normalVelocity) < (gravityStep * 2.0f)) {
            restitution = 0.0f; 
        }

		float impulseMagnitude = -(1.0f + restitution) * normalVelocity / totalInvMass;
		glm::vec3 linearImpulse = impulseMagnitude * normal;
		rbA->velocity -= linearImpulse * invMassA;
		rbB->velocity += linearImpulse * invMassB;
	}
};
}