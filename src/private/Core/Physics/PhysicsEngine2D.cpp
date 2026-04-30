// #include "Core/Physics/PhysicsEngine2D.h"

// #include "Core/Global.h"
// #include "Core/Components/Physics/2D/ACollider2DComponent.h"
// #include "Core/Components/Physics/2D/Rigidbody2DComponent.h"
// #include "Core/Components/Physics/2D/SquareColliderComponent.h"
// #include "Core/Objects/AObject.h"

// std::vector<Rigidbody2DComponent*> PhysicsEngine2D::rigidbodies =
// 	std::vector<Rigidbody2DComponent*>();

// std::vector<ACollider2DComponent*> PhysicsEngine2D::colliders =
// 	std::vector<ACollider2DComponent*>();

// void PhysicsEngine2D::Awake(std::vector<AObject*> objects) {
// 	for (AObject* object : objects) {
// 		AddObjectRigidbodyAndCollider(object);
// 	}
// }

// void PhysicsEngine2D::Update() {
// 	for (int i = 0; i < rigidbodies.size(); i++) {
// 		std::vector<ACollider2DComponent*> nearColliders = GetNearColliders(rigidbodies[i]);
// 		rigidbodies[i]->PhysicsUpdate(nearColliders, Global::FIXED_DELTA_TIME);
// 	}
// }

// void PhysicsEngine2D::AddObjectRigidbodyAndCollider(AObject* object) {
// 	for (IComponent* component : object->components) {

// 		Rigidbody2DComponent* rigidbody = dynamic_cast<Rigidbody2DComponent*>(component);
// 		if (rigidbody != nullptr) {
// 			AddRigidbody(rigidbody);
// 			continue;
// 		}

// 		ACollider2DComponent* collider = dynamic_cast<ACollider2DComponent*>(component);
// 		if (collider != nullptr) {
// 			AddCollider(collider);
// 			continue;
// 		}
// 	}

// 	for (AObject* child : object->children) {
// 		AddObjectRigidbodyAndCollider(child);
// 	}
// }

// void PhysicsEngine2D::AddRigidbody(Rigidbody2DComponent* rigidbody) {
// 	rigidbodies.push_back(rigidbody);
// }

// void PhysicsEngine2D::AddCollider(ACollider2DComponent* collider) {
// 	colliders.push_back(collider);
// }

// std::vector<ACollider2DComponent*> PhysicsEngine2D::GetNearColliders(Rigidbody2DComponent* rigidbody) {
// 	std::vector<ACollider2DComponent*> nearColliders = std::vector<ACollider2DComponent*>();

// 	// glm::vec2 maxDistance = rigidbody->velocity != glm::vec2(0) ?
// 	// 	rigidbody->velocity : rigidbody->parent->GetWorldScale();
	
// 	// for (ACollider2DComponent* collider : colliders) {
// 	// 	if (rigidbody->parent == collider->parent) continue;

// 	// 	glm::vec3 distance = rigidbody->parent->GetWorldPosition() - collider->parent->GetWorldPosition();
		
// 	// 	if (glm::length(distance) <= glm::length(maxDistance)) {
// 	// 		nearColliders.push_back(collider);
// 	// 	}
// 	// }

// 	return nearColliders;
// }

// void PhysicsEngine2D::CalcMinAndMax(glm::vec2& min, glm::vec2& max, glm::vec2 center, glm::vec2 halfSize) {
// 	min = center - halfSize;
// 	max = center + halfSize;
// }

// bool PhysicsEngine2D::PointIntersectsSquareCollider(glm::vec2 point, SquareColliderComponent* collider) {
// 	glm::vec2 min, max;
// 	CalcMinAndMax(min, max, collider->GetWorldCenter(), collider->GetWorldHalfSize());

// 	return (
// 		point.x >= min.x &&
// 		point.x <= max.x &&
// 		point.y >= min.y &&
// 		point.y <= max.y
// 	);
// }

// bool PhysicsEngine2D::SquareColliderIntesectsSquareCollider(
// 	SquareColliderComponent* collider1,
// 	SquareColliderComponent* collider2
// ) {
// 	glm::vec2 resultPosition = collider1->GetWorldCenter() - collider2->GetWorldCenter();
// 	glm::vec2 resultHalfSize = collider1->GetWorldHalfSize() + collider2->GetWorldHalfSize();

// 	glm::vec2 min, max;
// 	CalcMinAndMax(min, max, resultPosition, resultHalfSize);

// 	return (
// 		min.x <= 0 &&
// 		max.x >= 0 &&
// 		min.y <= 0 &&
// 		max.y >= 0
// 	);
// }

// glm::vec2 PhysicsEngine2D::SquareColliderPenetration(
// 	SquareColliderComponent* collider1,
// 	SquareColliderComponent* collider2
// ) {
// 	glm::vec2 resultPosition = collider1->GetWorldCenter() - collider2->GetWorldCenter();
// 	glm::vec2 resultHalfSize = collider1->GetWorldHalfSize() + collider2->GetWorldHalfSize();

// 	glm::vec2 min, max;
// 	CalcMinAndMax(min, max, resultPosition, resultHalfSize);

// 	glm::vec2 penetration = glm::vec2(min.x, 0);

// 	float minDistance = INFINITY;
	
// 	for (int i = 0; i < 2; i++) {
// 		if (std::abs(min[i]) < minDistance) {
// 			minDistance = std::abs(min[i]);
// 			penetration = glm::vec2(0);
// 			penetration[i] = min[i];
// 		}

// 		if (std::abs(max[i]) < minDistance) {
// 			penetration = glm::vec2(0);
// 			penetration[i] = max[i];
// 		}
// 	}
	
// 	return penetration;
// }

// PhysicsEngine2D::Raycast2D PhysicsEngine2D::RaycastSquareCollider(
// 	glm::vec2 position,
// 	glm::vec2 direction,
// 	double distance,
// 	glm::vec2 colliderPosition,
// 	glm::vec2 colliderHalfSize) {
	
// 	Raycast2D hit;
	
// 	direction = glm::normalize(direction);
	
// 	glm::vec2 magnitude = direction;
// 	magnitude *= distance;
	
// 	glm::vec2 min, max;
// 	CalcMinAndMax(min, max, colliderPosition, colliderHalfSize);

// 	float lastEntry = 0.0f;
// 	float firstExit = INFINITY;
	
// 	for (int i = 0; i < 2; i++) {
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