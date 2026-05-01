#include <Core/Physics/PhysicsEngine3D.h>


// #include <iostream>

// #include "Core/Global.h"
// #include "Core/Components/Physics/3D/ACollider3DComponent.h"
// #include "Core/Components/Physics/2D/SquareColliderComponent.h"
// #include "Core/Components/Physics/3D/BoxColliderComponent.h"
// #include "Core/Components/Physics/3D/Rigidbody3DComponent.h"
// #include "Core/Objects/AObject.h"

// std::vector<Rigidbody3DComponent*> PhysicsEngine3D::rigidbodies =
// 	std::vector<Rigidbody3DComponent*>();

// std::vector<ACollider3DComponent*> PhysicsEngine3D::colliders =
// 	std::vector<ACollider3DComponent*>();

// void PhysicsEngine3D::Awake(std::vector<AObject*> objects) {
// 	for (AObject* object : objects) {
// 		AddObjectRigidbodyAndCollider(object);
// 	}
// }

// void PhysicsEngine3D::Update() {
// 	for (int i = 0; i < rigidbodies.size(); i++) {
// 		std::vector<ACollider3DComponent*> nearColliders = GetNearColliders(rigidbodies[i]);
// 		rigidbodies[i]->PhysicsUpdate(nearColliders, Global::FIXED_DELTA_TIME);
// 	}
// }

// void PhysicsEngine3D::AddObjectRigidbodyAndCollider(AObject* object) {
// 	for (IComponent* component : object->components) {

// 		Rigidbody3DComponent* rigidbody = dynamic_cast<Rigidbody3DComponent*>(component);
// 		if (rigidbody != nullptr) {
// 			AddRigidbody(rigidbody);
// 			continue;
// 		}

// 		ACollider3DComponent* collider = dynamic_cast<ACollider3DComponent*>(component);
// 		if (collider != nullptr) {
// 			AddCollider(collider);
// 			continue;
// 		}
// 	}

// 	for (AObject* child : object->children) {
// 		AddObjectRigidbodyAndCollider(child);
// 	}
// }

// void PhysicsEngine3D::AddRigidbody(Rigidbody3DComponent* rigidbody) {
// 	rigidbodies.push_back(rigidbody);
// }

// void PhysicsEngine3D::AddCollider(ACollider3DComponent* collider) {
// 	colliders.push_back(collider);
// }

// std::vector<ACollider3DComponent*> PhysicsEngine3D::GetNearColliders(Rigidbody3DComponent* rigidbody) {
// 	std::vector<ACollider3DComponent*> nearColliders = std::vector<ACollider3DComponent*>();

// 	glm::vec3 maxDistance = rigidbody->velocity != glm::vec3(0) ?
// 		rigidbody->velocity : rigidbody->parent->GetWorldScale();
	
// 	for (ACollider3DComponent* collider : colliders) {
// 		if (rigidbody->parent == collider->parent) continue;

// 		glm::vec3 distance = rigidbody->parent->GetWorldPosition() - collider->parent->GetWorldPosition();
		
// 		if (glm::length(distance) <= glm::length(maxDistance)) {
// 			nearColliders.push_back(collider);
// 		}
// 	}

// 	return nearColliders;
// }

// void PhysicsEngine3D::CalcMinAndMax(glm::vec3& min, glm::vec3& max, glm::vec3 center, glm::vec3 halfSize) {
// 	min = center - halfSize;
// 	max = center + halfSize;
// }

// bool PhysicsEngine3D::PointIntersectsBoxCollider(glm::vec3 point, BoxColliderComponent* collider) {
// 	glm::vec3 min, max;
// 	CalcMinAndMax(min, max, collider->GetWorldCenter(), collider->GetWorldHalfSize());

// 	return (
// 		point.x >= min.x &&
// 		point.x <= max.x &&
// 		point.y >= min.y &&
// 		point.y <= max.y &&
// 		point.z >= min.z &&
// 		point.z <= max.z
// 	);
// }

// bool PhysicsEngine3D::BoxColliderIntesectsBoxCollider(
// 	BoxColliderComponent* collider1,
// 	BoxColliderComponent* collider2
// ) {
// 	glm::vec3 resultPosition = collider1->GetWorldCenter() - collider2->GetWorldCenter();
// 	glm::vec3 resultHalfSize = collider1->GetWorldHalfSize() + collider2->GetWorldHalfSize();

// 	glm::vec3 min, max;
// 	CalcMinAndMax(min, max, resultPosition, resultHalfSize);

// 	return (
// 		min.x <= 0 &&
// 		max.x >= 0 &&
// 		min.y <= 0 &&
// 		max.y >= 0 &&
// 		min.z <= 0 &&
// 		max.z >= 0
// 	);
// }

// glm::vec3 PhysicsEngine3D::BoxColliderPenetration(
// 	BoxColliderComponent* collider1,
// 	BoxColliderComponent* collider2
// ) {
// 	glm::vec3 resultPosition = collider1->GetWorldCenter() - collider2->GetWorldCenter();
// 	glm::vec3 resultHalfSize = collider1->GetWorldHalfSize() + collider2->GetWorldHalfSize();

// 	glm::vec3 min, max;
// 	CalcMinAndMax(min, max, resultPosition, resultHalfSize);

// 	glm::vec3 penetration = glm::vec3(min.x, 0, 0);

// 	float minDistance = INFINITY;

// 	for (int i = 0; i < 3; i++) {
// 		if (std::abs(min[i]) < minDistance) {
// 			minDistance = std::abs(min[i]);
// 			penetration = glm::vec3(0);
// 			penetration[i] = min[i];
// 		}

// 		if (std::abs(max[i]) < minDistance) {
// 			penetration = glm::vec3(0);
// 			penetration[i] = max[i];
// 		}
// 	}

// 	return penetration;
// }

// PhysicsEngine3D::Raycast3D PhysicsEngine3D::RaycastBoxCollider(
// 	glm::vec3 position,
// 	glm::vec3 direction,
// 	double distance,
// 	glm::vec3 colliderPosition,
// 	glm::vec3 colliderHalfSize) {
	
// 	Raycast3D hit;
	
// 	direction = glm::normalize(direction);
	
// 	glm::vec3 magnitude = direction;
// 	magnitude *= distance;
	
// 	glm::vec3 min, max;
// 	CalcMinAndMax(min, max, colliderPosition, colliderHalfSize);

// 	float lastEntry = 0.0f;
// 	float firstExit = INFINITY;
	
// 	for (int i = 0; i < 3; i++) {
// 		if (std::abs(magnitude[i]) != 0) {
// 			float t1 = (min[i] - position[i]) / magnitude[i];
// 			float t2 = (max[i] - position[i]) / magnitude[i];

// 			lastEntry = std::max(lastEntry, std::min(t1, t2));
// 			firstExit = std::min(firstExit, std::max(t1, t2));
// 		}
// 		else if (position[i] <= min[i] || position[i] >= max[i]) {
// 			return hit;
// 		}
// 	}

// 	if (firstExit > lastEntry && firstExit > 0 && lastEntry < 1) {
// 		hit.hit = true;
// 		hit.time = lastEntry;
// 		hit.position = position + magnitude * lastEntry;
// 	}

// 	return hit;
// }