#include <Core/Components/Physics/2D/ACollider2DComponent.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>
#include <Core/Global.h>
#include <Core/Objects/AObject.h>
#include <Core/Physics/Physics2D/PhysicsEngine.h>
#include <Core/Physics/Physics2D/PhysicObject.h>
#include <Core/Physics/Physics2D/Quadtree.h>
#include <Core/Render/Color.h>
#include <Util/Logger.h>
#include <algorithm>

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
			if(nearObject->baseObject == object->baseObject) 
				continue; 
			//TODO: If an object has multiple physics components, the object is added multiple times
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
				return true;
			}
		}
	}
	return false;
}

PhysicsEngine::CollisionManifold PhysicsEngine::EPA(
	ACollider* colliderA,
	ACollider* colliderB,
	std::vector<glm::vec3>& simplex) {

	auto SupportDifference = [](ACollider* a, ACollider* b, glm::vec3 dir) -> glm::vec3 {
		return a->GetSupportPoint(dir) - b->GetSupportPoint(-dir);
	};

	// Ensures the counter clockwise order using the Shoelace formula to calculate the area
	auto EnsureOrder = [](std::vector<glm::vec3>& poly) {
		float area = 0.0f;
		for (int i = 0; i < poly.size(); i++) {
			glm::vec3 a = poly[i];
			glm::vec3 b = poly[(i + 1) % poly.size()];
			area += (a.x * b.y - b.x * a.y);
		}
		if (area < 0)
			std::reverse(poly.begin(), poly.end());
	};

	auto EdgeNormal = [](glm::vec3 a, glm::vec3 b) -> glm::vec3 {
		glm::vec3 edge = b - a;
		return glm::normalize(glm::vec3(edge.y, -edge.x, 0));
	};

	auto EdgeDistance = [&EdgeNormal](glm::vec3 a, glm::vec3 b) -> float {
		glm::vec3 normal = EdgeNormal(a, b);
		return glm::dot(normal, a);
	};

	EnsureOrder(simplex);

	struct SupportPoint {
		glm::vec3 diff;
		glm::vec3 onA;
		glm::vec3 onB;
	};

	std::vector<SupportPoint> poly;
	for (auto& v : simplex) {
		glm::vec3 dir = glm::normalize(v);
		glm::vec3 onA = colliderA->GetSupportPoint(dir);
		glm::vec3 onB = colliderB->GetSupportPoint(-dir);
		poly.push_back({ v, onA, onB });
	}

	const float TOLERANCE = 1e-4f;

	while (true) {
		int closestIndex = 0;
		float closestDistance = FLT_MAX;
		glm::vec3 closestNormal;

		for (int j = 0; j < simplex.size(); j++) {
			glm::vec3 a = simplex[j];
			glm::vec3 b = simplex[(j + 1) % simplex.size()];

			float dist = EdgeDistance(a, b);
			glm::vec3 n = EdgeNormal(a, b);

			if (dist < closestDistance) {
				closestDistance = dist;
				closestNormal = n;
				closestIndex = j;
			}
		}

		glm::vec3 onA = colliderA->GetSupportPoint(closestNormal);
		glm::vec3 onB = colliderB->GetSupportPoint(-closestNormal);
		glm::vec3 support = onA - onB;

		float supportDist = glm::dot(support, closestNormal);

		if (supportDist - closestDistance < TOLERANCE) {

			const SupportPoint& spA = poly[closestIndex];
			const SupportPoint& spB = poly[(closestIndex + 1) % poly.size()];

			glm::vec3 edge = spB.diff - spA.diff;
			float edgeLenSq = glm::dot(edge, edge);
			float t = 0.5f;

			if (edgeLenSq > TOLERANCE) {
				t = glm::clamp(-glm::dot(spA.diff, edge) / edgeLenSq, 0.0f, 1.0f);
			}

			glm::vec3 contactOnA = spA.onA + t * (spB.onA - spA.onA);
			glm::vec3 contactOnB = spA.onB + t * (spB.onB - spA.onB);

			glm::vec3 contactPoint = (contactOnA + contactOnB) * 0.5f;

			return { closestNormal, closestDistance, contactPoint };
		}

		simplex.insert(simplex.begin() + closestIndex + 1, support);
		poly.insert(poly.begin() + closestIndex + 1, { support, onA, onB });
	}
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
	auto rbA = physicObjectA->rigidbody;
    auto rbB = physicObjectB->rigidbody;

    float invMassA = rbA->GetInverseMass();
    float invMassB = rbB->GetInverseMass();
    float totalInvMass = invMassA + invMassB;

	if (totalInvMass <= 0.0f)
		return;
	
	glm::vec2 normal = glm::vec2(manifold.PenetrationVector);

	const float slop = 0.01f;
    const float percent = 0.8f; 
    
    if (manifold.Depth > slop) {
        float correctionAmount = (manifold.Depth - slop) * percent;
        
        glm::vec2 correctionVector = normal * (correctionAmount / totalInvMass);

        physicObjectA->baseObject->Translate(glm::vec3(-correctionVector * invMassA, 0.0f));
        physicObjectB->baseObject->Translate(glm::vec3(correctionVector * invMassB, 0.0f));
    }

	glm::vec2 relativeVelocity = rbB->velocity - rbA->velocity;
    float normalVelocity = glm::dot(relativeVelocity, normal);

	if (normalVelocity < 0.0f) {
		float restitution = 0.9f; //TODO: restitution
		
		float gravityStep = glm::length(glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y)) * Global::FIXED_DELTA_TIME;
        if (abs(normalVelocity) < (gravityStep * 2.0f)) {
            restitution = 0.0f; 
        }

		float impulseMagnitude = -(1.0f + restitution) * normalVelocity / totalInvMass;
		glm::vec2 linearImpulse = impulseMagnitude * normal;
		rbA->velocity -= linearImpulse * invMassA;
		rbB->velocity += linearImpulse * invMassB;
	}
};
}