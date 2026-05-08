#include <Core/Physics/APhysicsEngine.h>
#include <Core/Objects/AObject.h>
#include <Core/Components/Physics/ACollider.h>

void APhysicsEngine::Awake(std::vector<AObject*> objects){
	for (auto* object : objects) {
		RegisterObject(object);
	}
}

void APhysicsEngine::UpdatePhysics() {
	MoveObjects();
	UpdateTree();
	CheckCollisions();
}

bool APhysicsEngine::GJKCheck(ACollider* colliderA, ACollider* colliderB){

	auto SupportDifference = [](ACollider* colliderA, ACollider* colliderB, glm::vec3 direction) -> glm::vec3 {
		return colliderA->GetSupportPoint(direction) - colliderB->GetSupportPoint(-direction);
	};

	glm::vec3 direction = glm::normalize(colliderB->GetCenter() - colliderA->GetCenter());
	std::vector<glm::vec3> simplex = { SupportDifference(colliderA, colliderB, direction) };

	direction = glm::normalize(-simplex[0]);
	
	while (true) {
		glm::vec3 targetPoint = SupportDifference(colliderA, colliderB, direction);
		if(glm::dot(targetPoint, direction) < 0) return false;
		simplex.push_back(targetPoint);
		if (HandleSimplex(simplex, direction)) return true;
	}
}

bool APhysicsEngine::HandleSimplex(std::vector<glm::vec3>& simplex, glm::vec3& direction){
	if (simplex.size() == 2) return LineCase(simplex, direction);
	return TriangleCase(simplex, direction);
}

bool APhysicsEngine::LineCase(std::vector<glm::vec3>& simplex, glm::vec3& direction){

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

bool APhysicsEngine::TriangleCase(std::vector<glm::vec3>& simplex, glm::vec3& direction){

	auto TripleProduct = [](glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC) -> glm::vec3 {
		return glm::cross(glm::cross(vectorA, vectorB), vectorC);
	};

	glm::vec3 pointA = simplex[2];
	glm::vec3 pointB = simplex[1];
	glm::vec3 pointC = simplex[0];

	glm::vec3 vectorAB = pointB - pointA;
	glm::vec3 vectorAC = pointC - pointA;
	glm::vec3 vectorAO = -pointA;

	glm::vec3 perpAB = TripleProduct(vectorAB, vectorAC, vectorAB);
	glm::vec3 perpAC = TripleProduct(vectorAC, vectorAB, vectorAC);

	if (glm::length(perpAB) < 1e-10f  || glm::length(perpAC) < 1e-10f)
        return true;
	
	if (glm::dot(perpAB, vectorAO) > 0) {
		simplex = { simplex[1], simplex[0]};
		direction =  glm::normalize(perpAB);
		return false;
	}

	if (glm::dot(perpAC, vectorAO) > 0) {
		simplex = { simplex[2], simplex[0]};
		direction =  glm::normalize(perpAC);
		return false;
	}
	
	return true;
}