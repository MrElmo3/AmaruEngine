#include <Core/Physics/PhysicsEngine2D.h>

#include <Core/Global.h>
#include <Core/Components/Physics/2D/ACollider2DComponent.h>
#include <Core/Components/Physics/2D/Rigidbody2DComponent.h>
#include <Core/Objects/AObject.h>
#include <Core/Render/Render.h>
#include <Core/Render/Color.h>

void PhysicsEngine2D::PhysicObject::UpdateLimits() {
	bottomLeft = glm::vec2(std::numeric_limits<float>::infinity());
	topRight = glm::vec2(-std::numeric_limits<float>::infinity());

	if(colliders.empty())return;

	for(auto collider : colliders){
		auto [cMin, cMax] = collider->GetAABBContainer();
		bottomLeft 	= glm::vec2(std::min(bottomLeft.x, cMin.x), std::min(bottomLeft.y, cMin.y));
		topRight 	= glm::vec2(std::max(topRight.x, cMax.x), std::max(topRight.y, cMax.y));
	}
}

void PhysicsEngine2D::RegisterObject(AObject* object){
	Rigidbody2DComponent* rigidbody = object->GetComponent<Rigidbody2DComponent>();
	
	if(rigidbody != nullptr && rigidbody->PhysicsEnabled()){
		PhysicObject* newPhysicObject = new PhysicObject();
		newPhysicObject->baseObject = object;
		newPhysicObject->rigidbody = rigidbody;
		newPhysicObject->colliders = object->GetAllComponentsInChildren<ACollider2DComponent>();
		newPhysicObject->UpdateLimits();
		physicObjects.push_back(newPhysicObject);
		return;
	}

	auto colliders = object->GetComponentsOfType<ACollider2DComponent>();
	if(!colliders.empty()){
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

void PhysicsEngine2D::MoveObjects() {
	for(auto physicObject : physicObjects){
		if(physicObject->rigidbody != nullptr)
			MoveRigidbody(physicObject->rigidbody);
	}
}

void PhysicsEngine2D::MoveRigidbody(Rigidbody2DComponent* rigidbody) {
	UpdateAceleration(rigidbody);
	UpdateVelocity(rigidbody);
	UpdatePosition(rigidbody);
}

void PhysicsEngine2D::UpdateAceleration(Rigidbody2DComponent* rigidbody) {
	rigidbody->forceAccumulator += glm::vec2(Global::GRAVITY.x, Global::GRAVITY.y) * rigidbody->gravityScale ;
	rigidbody->aceleration = rigidbody->forceAccumulator / rigidbody->mass;
	rigidbody->forceAccumulator = glm::vec3(0);
}

void PhysicsEngine2D::UpdateVelocity(Rigidbody2DComponent* rigidbody) {
	rigidbody->velocity += rigidbody->aceleration * Global::FIXED_DELTA_TIME;
}

void PhysicsEngine2D::UpdatePosition(Rigidbody2DComponent* rigidbody) {
	rigidbody->parent->Translate(glm::vec3(rigidbody->velocity * Global::FIXED_DELTA_TIME, 0.f));
}

void PhysicsEngine2D::UpdateTree() {
	UpdateContainers();
}

void PhysicsEngine2D::UpdateContainers() {
	for (auto physicObject : physicObjects) {
		physicObject->UpdateLimits();
	}
}

void PhysicsEngine2D::CheckCollisions() {
	// for(int i = 0; i < physicObjects)
}

	//	Update the Quad/Octree positions

	//	Check collisions
	//		GetNearColliders
	//		Check if the RB Colliders colied with one of the near colliders
	//		If so, ResolveColision
	//			Calculates Manifold
	//			Updates RB's velocity
	//			Repell RB



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