#include <Core/Components/Physics/ACollider.h>
#include <Core/Objects/AObject.h>
#include <Core/Physics/APhysicsEngine.h>
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

	auto manifold = APhysicsEngine::CollisionManifold();

	return manifold;
}
