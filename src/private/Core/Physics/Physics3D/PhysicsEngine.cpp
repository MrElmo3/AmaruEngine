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

APhysicsEngine::CollisionManifold APhysicsEngine::EPA(
	ACollider* colliderA,
	ACollider* colliderB,
	std::vector<glm::vec3>& simplex) {
	// EPA requires the input simplex to be a tetrahedron (4 points) that
	// already encloses the origin in Minkowski space -- that's GJK's job,
	// not EPA's. If this fires, the bug is in GJK's termination, not here.
	if (simplex.size() != 4) {
		Logger::Warning("APhysicsEngine::EPA expected a tetrahedron (4 points) from GJK.");
		return { glm::vec3(0, 1, 0), 0.0f, glm::vec3(0) };
	}

	struct Vert {
		glm::vec3 diff, onA, onB;
	};
	struct Face {
		int a, b, c;
		glm::vec3 normal;
		float dist;
	};

	auto Support = [&](glm::vec3 dir) -> Vert {
		glm::vec3 a = colliderA->GetSupportPoint(dir);
		glm::vec3 b = colliderB->GetSupportPoint(-dir);
		return { a - b, a, b };
	};

	std::vector<Vert> verts;
	for (auto& p : simplex) {
		// Reconstruct onA/onB for the initial tetrahedron points.
		// Assumes GJK's original support direction was ~normalize(p).
		// If GJK already tracks onA/onB per vertex, pass those through
		// instead -- cheaper and exact, see note below.
		glm::vec3 dir = glm::normalize(p);
		verts.push_back({ p, colliderA->GetSupportPoint(dir), colliderB->GetSupportPoint(-dir) });
	}

	// Builds a face with an outward-facing normal (away from origin),
	// regardless of the winding order passed in.
	auto MakeFace = [&](int i0, int i1, int i2) -> Face {
		glm::vec3 a = verts[i0].diff, b = verts[i1].diff, c = verts[i2].diff;
		glm::vec3 n = glm::cross(b - a, c - a);
		float len = glm::length(n);
		if (len < 1e-9f)
			return { i0, i1, i2, glm::vec3(0, 1, 0), FLT_MAX }; // degenerate guard
		n /= len;
		float d = glm::dot(n, a);
		if (d < 0.0f) {
			n = -n;
			d = -d;
			std::swap(i1, i2);
		}
		return { i0, i1, i2, n, d };
	};

	std::vector<Face> faces = {
		MakeFace(0, 1, 2),
		MakeFace(0, 3, 1),
		MakeFace(0, 2, 3),
		MakeFace(1, 3, 2),
	};

	const float TOLERANCE = 1e-4f;
	const int MAX_ITER = 64;

	for (int iter = 0; iter < MAX_ITER; ++iter) {
		int best = 0;
		for (int i = 1; i < (int)faces.size(); ++i)
			if (faces[i].dist < faces[best].dist)
				best = i;
		Face closest = faces[best];

		Vert sp = Support(closest.normal);
		float supportDist = glm::dot(sp.diff, closest.normal);

		if (supportDist - closest.dist < TOLERANCE) {
			// Barycentric projection of the origin onto the closest face
			glm::vec3 a = verts[closest.a].diff, b = verts[closest.b].diff, c = verts[closest.c].diff;
			glm::vec3 proj = closest.normal * closest.dist;
			glm::vec3 v0 = b - a, v1 = c - a, v2 = proj - a;
			float d00 = glm::dot(v0, v0), d01 = glm::dot(v0, v1), d11 = glm::dot(v1, v1);
			float d20 = glm::dot(v2, v0), d21 = glm::dot(v2, v1);
			float denom = d00 * d11 - d01 * d01;
			float v = (d11 * d20 - d01 * d21) / denom;
			float w = (d00 * d21 - d01 * d20) / denom;
			float u = 1.0f - v - w;

			glm::vec3 cA = u * verts[closest.a].onA + v * verts[closest.b].onA + w * verts[closest.c].onA;
			glm::vec3 cB = u * verts[closest.a].onB + v * verts[closest.b].onB + w * verts[closest.c].onB;
			return { closest.normal, closest.dist, (cA + cB) * 0.5f };
		}

		int newIdx = (int)verts.size();
		verts.push_back(sp);

		// Remove faces visible from the new point, collect the silhouette
		// (boundary edges), then patch the hole with new triangles.
		std::vector<std::pair<int, int>> silhouette;
		std::vector<Face> kept;
		for (auto& f : faces) {
			if (glm::dot(f.normal, sp.diff - verts[f.a].diff) > 0.0f) {
				auto addEdge = [&](int x, int y) {
					for (size_t k = 0; k < silhouette.size(); ++k) {
						if (silhouette[k].first == y && silhouette[k].second == x) {
							silhouette[k] = silhouette.back();
							silhouette.pop_back();
							return;
						}
					}
					silhouette.push_back({ x, y });
				};
				addEdge(f.a, f.b);
				addEdge(f.b, f.c);
				addEdge(f.c, f.a);
			} else {
				kept.push_back(f);
			}
		}
		for (auto& e : silhouette)
			kept.push_back(MakeFace(e.first, e.second, newIdx));
		faces = std::move(kept);
	}

	int best = 0;
	for (int i = 1; i < (int)faces.size(); ++i)
		if (faces[i].dist < faces[best].dist)
			best = i;
	return { faces[best].normal, faces[best].dist, glm::vec3(0) };
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