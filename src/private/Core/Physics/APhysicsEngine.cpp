#include <Core/Components/Physics/ACollider.h>
#include <Core/Objects/AObject.h>
#include <Core/Physics/APhysicsEngine.h>
#include <algorithm>
#include <glm/gtc/epsilon.hpp>

void APhysicsEngine::Awake(std::vector<AObject*> objects) {
	for (auto* object : objects) {
		RegisterObject(object);
	}
}

void APhysicsEngine::UpdatePhysics() {
	MoveObjects();
	UpdateTree();
	CheckCollisions();
}

APhysicsEngine::GJKResult APhysicsEngine::GJKCheck(ACollider* colliderA, ACollider* colliderB) {

	GJKResult result;

	auto SupportDifference = [](ACollider* colliderA, ACollider* colliderB, glm::vec3 direction) -> glm::vec3 {
		return colliderA->GetSupportPoint(direction) - colliderB->GetSupportPoint(-direction);
	};

	glm::vec3 direction = glm::normalize(colliderB->GetCenter() - colliderA->GetCenter());
	std::vector<glm::vec3> simplex = { SupportDifference(colliderA, colliderB, direction) };

	float epsilon = 0.0001f;
	
	if(glm::all(glm::epsilonEqual(simplex[0], glm::vec3(0), epsilon))){
		result.Collide = false;
		return result;
	}

	direction = glm::normalize(-simplex[0]);

	while (true) {
		glm::vec3 targetPoint = SupportDifference(colliderA, colliderB, direction);
		if (glm::dot(targetPoint, direction) < 0) {
			result.Collide = false;
			return result;
		};

		simplex.push_back(targetPoint);

		if (HandleSimplex(simplex, direction)) {
			result.Collide = true;
			result.Simplex = simplex;
			return result;
		};
	}
}

bool APhysicsEngine::HandleSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction) {
	if (simplex.size() == 2)
		return LineCase(simplex, direction);
	if (simplex.size() == 3)
		return TriangleCase(simplex, direction);
	return TetrahedronCase(simplex, direction);
}

bool APhysicsEngine::LineCase(std::vector<glm::vec3>& simplex, glm::vec3& direction) {

	auto TripleProduct = [](glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC) -> glm::vec3 {
		return glm::cross(glm::cross(vectorA, vectorB), vectorC);
	};

	glm::vec3 pointA = simplex[1];
	glm::vec3 pointB = simplex[0];

	glm::vec3 vectorAB = pointB - pointA;
	glm::vec3 vectorAO = -pointA;

	glm::vec3 perpAB = TripleProduct(vectorAB, vectorAO, vectorAB);

	if (glm::length(perpAB) < 1e-10f)
		return true;

	direction = glm::normalize(perpAB);
	return false;
}

bool APhysicsEngine::TriangleCase(std::vector<glm::vec3>& simplex, glm::vec3& direction) {

	auto TripleProduct = [](glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC) -> glm::vec3 {
		return glm::cross(glm::cross(vectorA, vectorB), vectorC);
	};

	glm::vec3 pointA = simplex[2];
	glm::vec3 pointB = simplex[1];
	glm::vec3 pointC = simplex[0];

	glm::vec3 vectorAB = pointB - pointA;
	glm::vec3 vectorAC = pointC - pointA;
	glm::vec3 vectorAO = -pointA;

	glm::vec3 perpABC = glm::cross(vectorAB, vectorAC);
	glm::vec3 perpAB = TripleProduct(vectorAC, vectorAB, vectorAB);
	glm::vec3 perpAC = TripleProduct(vectorAB, vectorAC, vectorAC);

	if (glm::dot(perpABC, vectorAO) == 0.f) {
		if (glm::length(perpAB) < 1e-10f || glm::length(perpAC) < 1e-10f)
			return true;
		if (glm::dot(perpAB, vectorAO) > 0) {
			simplex = { simplex[1], simplex[2] };
			direction = glm::normalize(perpAB);
			return false;
		}
		if (glm::dot(perpAC, vectorAO) > 0) {
			simplex = { simplex[2], simplex[0] };
			direction = glm::normalize(perpAC);
			return false;
		}
		return true;
	}

	if (glm::dot(perpABC, vectorAO) > 0) {
		direction = glm::normalize(perpABC);
		return false;
	} else {
		direction = glm::normalize(-perpABC);
		return false;
	}
}

bool APhysicsEngine::TetrahedronCase(std::vector<glm::vec3>& simplex, glm::vec3& direction) {

	glm::vec3 pointA = simplex[3];
	glm::vec3 pointB = simplex[2];
	glm::vec3 pointC = simplex[1];
	glm::vec3 pointD = simplex[0];

	glm::vec3 vectorAB = pointB - pointA;
	glm::vec3 vectorAC = pointC - pointA;
	glm::vec3 vectorAD = pointD - pointA;
	glm::vec3 vectorAO = -pointA;

	glm::vec3 perpBAC = glm::cross(vectorAB, vectorAC);
	glm::vec3 perpCAD = glm::cross(vectorAC, vectorAD);
	glm::vec3 perpDAB = glm::cross(vectorAD, vectorAB);

	if (glm::dot(perpBAC, vectorAD) > 0)
		perpBAC = -perpBAC;
	if (glm::dot(perpCAD, vectorAB) > 0)
		perpCAD = -perpCAD;
	if (glm::dot(perpDAB, vectorAC) > 0)
		perpDAB = -perpDAB;

	if (glm::length(perpBAC) < 1e-10f || glm::length(perpCAD) < 1e-10f || glm::length(perpDAB) < 1e-10f)
		return true;

	if (glm::dot(perpBAC, vectorAO) > 0) {
		simplex = { simplex[1], simplex[2], simplex[3] };
		direction = glm::normalize(perpBAC);
		return false;
	}
	if (glm::dot(perpCAD, vectorAO) > 0) {
		simplex = { simplex[3], simplex[1], simplex[0] };
		direction = glm::normalize(perpCAD);
		return false;
	}

	if (glm::dot(perpDAB, vectorAO) > 0) {
		simplex = { simplex[3], simplex[2], simplex[0] };
		direction = glm::normalize(perpCAD);
		return false;
	}

	return true;
}


//TODO: implement EPA for 3D polytopes.
APhysicsEngine::CollisionManifold APhysicsEngine::EPA(
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
