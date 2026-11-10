#include <Core/Physics/Physics3D/PhysicsEngine.h>
#include <Core/Physics/Physics3D/PhysicObject.h>
#include <Core/Physics/Physics3D/Octree.h>
#include <Core/Objects/AObject.h>
#include <Core/Components/Physics/3D/ACollider3DComponent.h>
#include <Core/Components/Physics/3D/Rigidbody3DComponent.h>
#include <Core/Render/Color.h>
#include <Core/Global.h>

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <functional>
#include <utility>

namespace Physics3D {

namespace {
	const int GJK_MAX_ITERATIONS = 64;
	const int EPA_MAX_ITERATIONS = 64;
	const float EPA_TOLERANCE = 1e-4f;
	const float EPSILON = 1e-10f;

	/// Penetration allowed before pushing the objects apart (avoids jitter on resting contacts).
	const float PENETRATION_SLOP = 0.005f;
	/// Fraction of the penetration that is corrected each step.
	const float CORRECTION_PERCENT = 0.8f;

	glm::vec3 TripleCross(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		return glm::cross(glm::cross(a, b), c);
	}

	/// Returns any vector perpendicular to v.
	glm::vec3 AnyPerpendicular(glm::vec3 v) {
		glm::vec3 axis = std::abs(v.x) < 0.57735f ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
		return glm::cross(v, axis);
	}

	bool AabbOverlap(const PhysicObject* a, const PhysicObject* b) {
		return
			a->bottomLeft.x <= b->topRight.x && a->topRight.x >= b->bottomLeft.x &&
			a->bottomLeft.y <= b->topRight.y && a->topRight.y >= b->bottomLeft.y &&
			a->bottomLeft.z <= b->topRight.z && a->topRight.z >= b->bottomLeft.z;
	}

	/// Returns the rigidbody only if it exists and is enabled; otherwise the object is treated as static.
	Rigidbody3DComponent* ActiveRigidbody(const PhysicObject* physicObject) {
		Rigidbody3DComponent* rigidbody = physicObject->rigidbody;
		if (rigidbody == nullptr || !rigidbody->IsEnabled()) return nullptr;
		return rigidbody;
	}
}

PhysicsEngine::PhysicsEngine() {
	octree = new Octree();
}

PhysicsEngine::~PhysicsEngine() {
	for (auto* physicObject : physicObjects)
		delete physicObject;
	delete octree;
}

//----------------------------------------------------------------------------
// Registration
//----------------------------------------------------------------------------

void PhysicsEngine::RegisterObject(AObject* object) {
	// Every physics component calls this for its parent, so the same object
	// arrives several times. Only the first call creates a PhysicObject.
	if (object == nullptr || registeredObjects.count(object) > 0)
		return;

	Rigidbody3DComponent* rigidbody = object->GetComponent<Rigidbody3DComponent>();

	if (rigidbody != nullptr && rigidbody->PhysicsEnabled()) {
		// A rigidbody owns the colliders of its object and of all of its
		// children (compound body), so the children must not be registered
		// again as independent static objects.
		PhysicObject* newPhysicObject = new PhysicObject();
		newPhysicObject->baseObject = object;
		newPhysicObject->rigidbody = rigidbody;
		newPhysicObject->colliders = object->GetAllComponentsInChildren<ACollider3DComponent>();
		newPhysicObject->UpdateLimits();
		physicObjects.push_back(newPhysicObject);

		std::function<void(AObject*)> markRegistered = [&](AObject* current) {
			registeredObjects.insert(current);
			for (auto* child : current->GetChildren())
				markRegistered(child);
		};
		markRegistered(object);
		return;
	}

	// Static object: only its own colliders. Its children register themselves
	// through their own physics components.
	auto colliders = object->GetComponentsOfType<ACollider3DComponent>();
	if (colliders.empty())
		return;

	PhysicObject* newPhysicObject = new PhysicObject();
	newPhysicObject->baseObject = object;
	newPhysicObject->rigidbody = nullptr;
	newPhysicObject->colliders = colliders;
	newPhysicObject->UpdateLimits();
	physicObjects.push_back(newPhysicObject);
	registeredObjects.insert(object);
}

//----------------------------------------------------------------------------
// Integration (linear only)
//----------------------------------------------------------------------------

void PhysicsEngine::MoveObjects() {
	for (auto* physicObject : physicObjects) {
		Rigidbody3DComponent* rigidbody = ActiveRigidbody(physicObject);
		if (rigidbody != nullptr)
			MoveRigidbody(rigidbody);
	}
}

void PhysicsEngine::MoveRigidbody(Rigidbody3DComponent* rigidbody) {
	const float dt = Global::FIXED_DELTA_TIME;

	// Gravity is an acceleration: it must not be divided by the mass.
	rigidbody->acceleration =
		rigidbody->forceAccumulator * rigidbody->GetInverseMass() +
		Global::GRAVITY * rigidbody->gravityScale;
	rigidbody->forceAccumulator = glm::vec3(0);

	// Semi-implicit Euler: velocity first, then position with the new velocity.
	rigidbody->velocity += rigidbody->acceleration * dt;

	if (rigidbody->velocity != glm::vec3(0))
		rigidbody->parent->TranslateWorld(rigidbody->velocity * dt);
}

//----------------------------------------------------------------------------
// Broad phase
//----------------------------------------------------------------------------

void PhysicsEngine::UpdateTree() {
	glm::vec3 worldMin = glm::vec3(FLT_MAX);
	glm::vec3 worldMax = glm::vec3(-FLT_MAX);

	for (auto* physicObject : physicObjects) {
		physicObject->UpdateLimits();

		// Objects without colliders have inverted limits: they don't take part in collisions.
		if (physicObject->bottomLeft.x > physicObject->topRight.x)
			continue;

		worldMin = glm::min(worldMin, physicObject->bottomLeft);
		worldMax = glm::max(worldMax, physicObject->topRight);
	}

	if (worldMin.x > worldMax.x) {
		octree->Clear();
		return;
	}

	// The tree always fits the current objects, so nothing is left outside of it.
	const glm::vec3 margin = glm::vec3(1.0f);
	octree->Build(physicObjects, worldMin - margin, worldMax + margin);
}

//----------------------------------------------------------------------------
// Narrow phase + resolution
//----------------------------------------------------------------------------

void PhysicsEngine::CheckCollisions() {
	for (auto* physicObject : physicObjects) {
		for (auto* collider : physicObject->colliders)
			collider->debugColor = Color::GREEN;
	}

	std::less<PhysicObject*> isBefore;
	std::vector<PhysicObject*> nearObjects;

	for (auto* object : physicObjects) {
		if (ActiveRigidbody(object) == nullptr || object->colliders.empty())
			continue;

		nearObjects.clear();
		octree->GetNearObjects(object, nearObjects);

		for (auto* nearObject : nearObjects) {
			// A pair of rigidbodies is visited from both sides: solve it only once.
			if (ActiveRigidbody(nearObject) != nullptr && isBefore(nearObject, object))
				continue;

			// Earlier resolutions may have moved the objects: refresh their limits.
			object->UpdateLimits();
			nearObject->UpdateLimits();
			if (!AabbOverlap(object, nearObject))
				continue;

			CheckCollision(object, nearObject);
		}
	}
}

void PhysicsEngine::CheckCollision(PhysicObject* physicObjectA, PhysicObject* physicObjectB) {
	for (auto* colliderA : physicObjectA->colliders) {
		if (!colliderA->IsEnabled()) continue;

		for (auto* colliderB : physicObjectB->colliders) {
			if (!colliderB->IsEnabled()) continue;

			Simplex simplex;
			if (!GJK(colliderA, colliderB, simplex))
				continue;

			colliderA->debugColor = Color::RED;
			colliderB->debugColor = Color::RED;

			// Triggers detect the overlap but don't push.
			if (colliderA->isTrigger || colliderB->isTrigger)
				continue;

			CollisionManifold manifold = EPA3D(colliderA, colliderB, simplex);
			if (manifold.Depth <= 0.0f)
				continue;

			// Every colliding pair is solved (compound bodies can have several contacts).
			ResolveCollision(physicObjectA, physicObjectB, manifold);
		}
	}
}

void PhysicsEngine::ResolveCollision(
	PhysicObject* physicObjectA,
	PhysicObject* physicObjectB,
	const CollisionManifold& manifold) {

	Rigidbody3DComponent* rbA = ActiveRigidbody(physicObjectA);
	Rigidbody3DComponent* rbB = ActiveRigidbody(physicObjectB);

	float invMassA = rbA != nullptr ? rbA->GetInverseMass() : 0.0f;
	float invMassB = rbB != nullptr ? rbB->GetInverseMass() : 0.0f;
	float totalInvMass = invMassA + invMassB;

	if (totalInvMass <= 0.0f)
		return;

	// Points from A to B: A must move along -normal and B along +normal.
	glm::vec3 normal = manifold.PenetrationVector;

	// 1) Positional correction, split by inverse mass.
	float correctionDepth = std::max(manifold.Depth - PENETRATION_SLOP, 0.0f) * CORRECTION_PERCENT;
	if (correctionDepth > 0.0f) {
		glm::vec3 correction = normal * (correctionDepth / totalInvMass);
		if (rbA != nullptr) physicObjectA->baseObject->TranslateWorld(-correction * invMassA);
		if (rbB != nullptr) physicObjectB->baseObject->TranslateWorld(correction * invMassB);
	}

	// 2) Normal impulse.
	glm::vec3 velocityA = rbA != nullptr ? rbA->velocity : glm::vec3(0);
	glm::vec3 velocityB = rbB != nullptr ? rbB->velocity : glm::vec3(0);

	glm::vec3 relativeVelocity = velocityB - velocityA;
	float normalVelocity = glm::dot(relativeVelocity, normal);

	// Already separating: nothing to do.
	if (normalVelocity >= 0.0f)
		return;

	float restitution =
		rbA != nullptr && rbB != nullptr ? std::min(rbA->restitution, rbB->restitution) :
		rbA != nullptr ? rbA->restitution : rbB->restitution;

	// Resting contact: approach speeds similar to one step of gravity must not bounce.
	float restingSpeed = 2.0f * glm::length(Global::GRAVITY) * Global::FIXED_DELTA_TIME;
	if (-normalVelocity < restingSpeed)
		restitution = 0.0f;

	float normalImpulse = -(1.0f + restitution) * normalVelocity / totalInvMass;
	velocityA -= normal * (normalImpulse * invMassA);
	velocityB += normal * (normalImpulse * invMassB);

	// 3) Friction impulse (Coulomb), along the tangential relative velocity.
	relativeVelocity = velocityB - velocityA;
	glm::vec3 tangent = relativeVelocity - glm::dot(relativeVelocity, normal) * normal;
	float tangentLength = glm::length(tangent);

	if (tangentLength > 1e-6f) {
		tangent /= tangentLength;

		float friction =
			rbA != nullptr && rbB != nullptr ? std::sqrt(rbA->friction * rbB->friction) :
			rbA != nullptr ? rbA->friction : rbB->friction;

		float tangentImpulse = -glm::dot(relativeVelocity, tangent) / totalInvMass;
		float maxFriction = normalImpulse * friction;
		tangentImpulse = glm::clamp(tangentImpulse, -maxFriction, maxFriction);

		velocityA -= tangent * (tangentImpulse * invMassA);
		velocityB += tangent * (tangentImpulse * invMassB);
	}

	if (rbA != nullptr) rbA->velocity = velocityA;
	if (rbB != nullptr) rbB->velocity = velocityB;
}

//----------------------------------------------------------------------------
// GJK
//----------------------------------------------------------------------------

PhysicsEngine::SupportPoint PhysicsEngine::Support(
	ACollider3DComponent* colliderA,
	ACollider3DComponent* colliderB,
	glm::vec3 direction) {

	SupportPoint support;
	support.onA = colliderA->GetSupportPoint(direction);
	support.onB = colliderB->GetSupportPoint(-direction);
	support.point = support.onA - support.onB;
	return support;
}

bool PhysicsEngine::GJK(ACollider3DComponent* colliderA, ACollider3DComponent* colliderB, Simplex& simplex) {
	glm::vec3 direction = colliderB->GetWorldPosition() - colliderA->GetWorldPosition();
	if (glm::dot(direction, direction) < EPSILON)
		direction = glm::vec3(1, 0, 0);

	simplex.points[0] = Support(colliderA, colliderB, direction);
	simplex.size = 1;

	direction = -simplex.points[0].point;
	if (glm::dot(direction, direction) < EPSILON)
		direction = AnyPerpendicular(simplex.points[0].point + glm::vec3(1, 0, 0));

	for (int iteration = 0; iteration < GJK_MAX_ITERATIONS; iteration++) {
		SupportPoint newPoint = Support(colliderA, colliderB, direction);

		// The new point didn't pass the origin: the Minkowski difference
		// can't contain it, so the shapes are separated.
		if (glm::dot(newPoint.point, direction) <= 0.0f)
			return false;

		for (int i = simplex.size; i > 0; i--)
			simplex.points[i] = simplex.points[i - 1];
		simplex.points[0] = newPoint;
		simplex.size++;

		if (DoSimplex(simplex, direction))
			return true;

		if (glm::dot(direction, direction) < EPSILON)
			return false;
	}

	// Did not converge (degenerate case): treat it as no collision.
	return false;
}

bool PhysicsEngine::DoSimplex(Simplex& simplex, glm::vec3& direction) {
	switch (simplex.size) {
		case 2: return LineCase(simplex, direction);
		case 3: return TriangleCase(simplex, direction);
		case 4: return TetrahedronCase(simplex, direction);
		default: return false;
	}
}

bool PhysicsEngine::LineCase(Simplex& simplex, glm::vec3& direction) {
	glm::vec3 a = simplex.points[0].point;
	glm::vec3 b = simplex.points[1].point;

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (glm::dot(ab, ao) > 0.0f) {
		direction = TripleCross(ab, ao, ab);
		// The origin is on the segment: search in any perpendicular direction.
		if (glm::dot(direction, direction) < EPSILON)
			direction = AnyPerpendicular(ab);
	}
	else {
		simplex.size = 1;
		direction = ao;
	}
	return false;
}

bool PhysicsEngine::TriangleCase(Simplex& simplex, glm::vec3& direction) {
	SupportPoint pointA = simplex.points[0];
	SupportPoint pointB = simplex.points[1];
	SupportPoint pointC = simplex.points[2];

	glm::vec3 a = pointA.point;
	glm::vec3 ab = pointB.point - a;
	glm::vec3 ac = pointC.point - a;
	glm::vec3 ao = -a;
	glm::vec3 abc = glm::cross(ab, ac);

	// Collinear points: drop C and continue as a line.
	if (glm::dot(abc, abc) < EPSILON) {
		simplex.size = 2;
		return LineCase(simplex, direction);
	}

	if (glm::dot(glm::cross(abc, ac), ao) > 0.0f) {
		if (glm::dot(ac, ao) > 0.0f) {
			// Region of edge AC
			simplex.points[1] = pointC;
			simplex.size = 2;
			direction = TripleCross(ac, ao, ac);
			if (glm::dot(direction, direction) < EPSILON)
				direction = AnyPerpendicular(ac);
			return false;
		}
		simplex.size = 2; // {A, B}
		return LineCase(simplex, direction);
	}

	if (glm::dot(glm::cross(ab, abc), ao) > 0.0f) {
		simplex.size = 2; // {A, B}
		return LineCase(simplex, direction);
	}

	// The origin is above or below the triangle.
	if (glm::dot(abc, ao) > 0.0f) {
		direction = abc;
	}
	else {
		// Swap the winding so the triangle faces the origin.
		simplex.points[1] = pointC;
		simplex.points[2] = pointB;
		direction = -abc;
	}
	return false;
}

bool PhysicsEngine::TetrahedronCase(Simplex& simplex, glm::vec3& direction) {
	SupportPoint pointA = simplex.points[0];
	SupportPoint pointB = simplex.points[1];
	SupportPoint pointC = simplex.points[2];
	SupportPoint pointD = simplex.points[3];

	glm::vec3 a = pointA.point;
	glm::vec3 ab = pointB.point - a;
	glm::vec3 ac = pointC.point - a;
	glm::vec3 ad = pointD.point - a;
	glm::vec3 ao = -a;

	// Flat tetrahedron: drop the oldest point and keep searching.
	if (std::abs(glm::dot(ab, glm::cross(ac, ad))) < EPSILON) {
		simplex.size = 3;
		return TriangleCase(simplex, direction);
	}

	glm::vec3 abc = glm::cross(ab, ac);
	glm::vec3 acd = glm::cross(ac, ad);
	glm::vec3 adb = glm::cross(ad, ab);

	// Orient every face normal outwards (away from the opposite vertex).
	if (glm::dot(abc, ad) > 0.0f) abc = -abc;
	if (glm::dot(acd, ab) > 0.0f) acd = -acd;
	if (glm::dot(adb, ac) > 0.0f) adb = -adb;

	if (glm::dot(abc, ao) > 0.0f) {
		simplex.size = 3; // {A, B, C}
		return TriangleCase(simplex, direction);
	}

	if (glm::dot(acd, ao) > 0.0f) {
		simplex.points[1] = pointC;
		simplex.points[2] = pointD;
		simplex.size = 3; // {A, C, D}
		return TriangleCase(simplex, direction);
	}

	if (glm::dot(adb, ao) > 0.0f) {
		simplex.points[1] = pointD;
		simplex.points[2] = pointB;
		simplex.size = 3; // {A, D, B}
		return TriangleCase(simplex, direction);
	}

	// The origin is inside the tetrahedron.
	return true;
}

//----------------------------------------------------------------------------
// EPA
//----------------------------------------------------------------------------

APhysicsEngine::CollisionManifold PhysicsEngine::EPA3D(
	ACollider3DComponent* colliderA,
	ACollider3DComponent* colliderB,
	const Simplex& simplex) {

	struct Face {
		int a, b, c;
		glm::vec3 normal;
		float distance;
	};

	std::vector<SupportPoint> vertices(simplex.points, simplex.points + 4);

	// A point strictly inside the polytope; it stays inside while the polytope grows.
	const glm::vec3 interior =
		(vertices[0].point + vertices[1].point + vertices[2].point + vertices[3].point) * 0.25f;

	// Builds a face whose normal points outwards (away from the interior point)
	// keeping a consistent winding, so shared edges appear reversed in neighbours.
	auto MakeFace = [&vertices, &interior](int ia, int ib, int ic) -> Face {
		glm::vec3 a = vertices[ia].point;
		glm::vec3 b = vertices[ib].point;
		glm::vec3 c = vertices[ic].point;

		glm::vec3 normal = glm::cross(b - a, c - a);
		float length = glm::length(normal);
		if (length < 1e-12f)
			return { ia, ib, ic, glm::vec3(0), FLT_MAX }; // degenerate: never selected

		normal /= length;
		if (glm::dot(normal, a - interior) < 0.0f) {
			normal = -normal;
			std::swap(ib, ic);
		}
		return { ia, ib, ic, normal, glm::dot(normal, a) };
	};

	auto ClosestFace = [](const std::vector<Face>& faces) -> int {
		int closest = 0;
		for (int i = 1; i < (int)faces.size(); i++) {
			if (faces[i].distance < faces[closest].distance)
				closest = i;
		}
		return closest;
	};

	auto BuildManifold = [&vertices](const Face& face) -> CollisionManifold {
		// Barycentric coordinates of the origin projected on the face, used to
		// interpolate the contact point on each shape.
		glm::vec3 projection = face.normal * face.distance;
		glm::vec3 a = vertices[face.a].point;
		glm::vec3 v0 = vertices[face.b].point - a;
		glm::vec3 v1 = vertices[face.c].point - a;
		glm::vec3 v2 = projection - a;

		float d00 = glm::dot(v0, v0);
		float d01 = glm::dot(v0, v1);
		float d11 = glm::dot(v1, v1);
		float d20 = glm::dot(v2, v0);
		float d21 = glm::dot(v2, v1);
		float denominator = d00 * d11 - d01 * d01;

		float u = 1.0f, v = 0.0f, w = 0.0f;
		if (std::abs(denominator) > 1e-12f) {
			v = (d11 * d20 - d01 * d21) / denominator;
			w = (d00 * d21 - d01 * d20) / denominator;
			u = 1.0f - v - w;
		}

		glm::vec3 contactA =
			u * vertices[face.a].onA + v * vertices[face.b].onA + w * vertices[face.c].onA;
		glm::vec3 contactB =
			u * vertices[face.a].onB + v * vertices[face.b].onB + w * vertices[face.c].onB;

		CollisionManifold manifold;
		manifold.PenetrationVector = face.normal;
		manifold.Depth = std::max(face.distance, 0.0f);
		manifold.ContactPoint = (contactA + contactB) * 0.5f;
		return manifold;
	};

	std::vector<Face> faces = {
		MakeFace(0, 1, 2),
		MakeFace(0, 3, 1),
		MakeFace(0, 2, 3),
		MakeFace(1, 3, 2),
	};

	for (int iteration = 0; iteration < EPA_MAX_ITERATIONS; iteration++) {
		const Face closest = faces[ClosestFace(faces)];
		if (closest.distance == FLT_MAX)
			break;

		SupportPoint support = Support(colliderA, colliderB, closest.normal);
		float supportDistance = glm::dot(support.point, closest.normal);

		// The polytope can't grow further in this direction: closest face found.
		if (supportDistance - closest.distance < EPA_TOLERANCE)
			return BuildManifold(closest);

		int newIndex = (int)vertices.size();
		vertices.push_back(support);

		// Remove every face visible from the new point and keep the border
		// (edges that belong to only one removed face) to close the hole.
		std::vector<std::pair<int, int>> borderEdges;
		auto AddEdge = [&borderEdges](int from, int to) {
			for (size_t i = 0; i < borderEdges.size(); i++) {
				if (borderEdges[i].first == to && borderEdges[i].second == from) {
					borderEdges.erase(borderEdges.begin() + i);
					return;
				}
			}
			borderEdges.push_back({ from, to });
		};

		std::vector<Face> remainingFaces;
		for (const Face& face : faces) {
			if (glm::dot(face.normal, support.point - vertices[face.a].point) > 0.0f) {
				AddEdge(face.a, face.b);
				AddEdge(face.b, face.c);
				AddEdge(face.c, face.a);
			}
			else {
				remainingFaces.push_back(face);
			}
		}

		for (const auto& edge : borderEdges)
			remainingFaces.push_back(MakeFace(edge.first, edge.second, newIndex));

		if (remainingFaces.empty())
			break;

		faces = std::move(remainingFaces);
	}

	// Did not converge: use the best approximation found.
	const Face closest = faces[ClosestFace(faces)];
	if (closest.distance == FLT_MAX) {
		CollisionManifold empty;
		empty.PenetrationVector = glm::vec3(0, 1, 0);
		empty.Depth = 0.0f;
		empty.ContactPoint = glm::vec3(0);
		return empty;
	}
	return BuildManifold(closest);
}
}
