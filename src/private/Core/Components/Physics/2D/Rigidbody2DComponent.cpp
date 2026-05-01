#include "Core/Components/Physics/2D/Rigidbody2DComponent.h"

#include "Core/Components/Physics/2D/ACollider2DComponent.h"
#include "Core/Components/Physics/2D/SquareColliderComponent.h"
#include "Core/Objects/AObject.h"
#include "Core/Physics/PhysicsEngine2D.h"

Rigidbody2DComponent::Rigidbody2DComponent(AObject* parent) : APhysics2DComponent(parent) {
	// velocity = glm::vec2(0.0f);
}

Rigidbody2DComponent::~Rigidbody2DComponent() {
}

void Rigidbody2DComponent::Awake() {
	APhysics2DComponent::AwakePhysics();
	// for (IComponent* component : parent->components) {
	// 	ACollider2DComponent* collider = dynamic_cast<ACollider2DComponent*>(component);
	// 	if (collider != nullptr) {
	// 		AddCollider(collider);
	// 	}
	// }
}

void Rigidbody2DComponent::End() {
	// colliders.clear();
}

void Rigidbody2DComponent::PhysicsUpdate(float fixedDeltaTime) {
	
	// for (ACollider2DComponent* externalCollider : nearColliders) {
	// 	for (ACollider2DComponent* rbCollider: colliders) {
	// 		DetectCollision(externalCollider, rbCollider, fixedDeltaTime);
	// 	}
	// }
	
	// if (velocity != glm::vec2(0))
	// 	parent->SetWorldPosition(parent->GetWorldPosition() + glm::vec3(velocity, 0) * fixedDeltaTime);
}

void Rigidbody2DComponent::DetectCollision(
	ACollider2DComponent* externalCollider,
	ACollider2DComponent* rbCollider, 
	float fixedDeltaTime) {
	
	// if (externalCollider->isTrigger || rbCollider->isTrigger) return;

	// SquareColliderComponent* externalSquareCollider = dynamic_cast<SquareColliderComponent*>(externalCollider);
	// SquareColliderComponent* rbSquareCollider = dynamic_cast<SquareColliderComponent*>(rbCollider);

	// if (externalSquareCollider == nullptr || rbSquareCollider == nullptr) return;

	// //simulated collider for the collision detection
	// glm::vec2 sumCenter = externalSquareCollider->GetWorldCenter();
	// glm::vec2 sumHalfSize = (
	// 	externalSquareCollider->GetWorldHalfSize() +
	// 	rbSquareCollider->GetWorldHalfSize()
	// 	);
	
	// for (int i = 0; i < 2; i++) {
	// 	if (velocity[i] != .0f) {
			
	// 		glm::vec2 direction = glm::vec3(0);
	// 		direction[i] = velocity[i];
			
	// 		PhysicsEngine2D::Raycast2D hit = PhysicsEngine2D::RaycastSquareCollider(
	// 			rbSquareCollider->GetWorldCenter(),
	// 			direction,
	// 			glm::length(velocity) * fixedDeltaTime,
	// 			sumCenter,
	// 			sumHalfSize
	// 		);
	// 		if (hit.hit) {
	// 			parent->SetWorldPosition(glm::vec3(hit.position, .0f));
	// 			velocity[i] = 0;
	// 		}
	// 	}
	// }
	
	// if (velocity == glm::vec2(0)) {
	// 	if (PhysicsEngine2D::SquareColliderIntesectsSquareCollider(
	// 		externalSquareCollider, rbSquareCollider)) {
			
	// 		glm::vec2 penetration = PhysicsEngine2D::SquareColliderPenetration(
	// 			externalSquareCollider, rbSquareCollider);
			
	// 		parent->SetWorldPosition(parent->GetWorldPosition() + glm::vec3(penetration, 0) * 1.01f);
	// 	}
	// }
}

void Rigidbody2DComponent::AddCollider(ACollider2DComponent* collider) {
	// colliders.push_back(collider);
}

void Rigidbody2DComponent::RemoveCollider(ACollider2DComponent* collider) {
	// for (int i = 0; i < colliders.size(); i++) {
	// 	if (colliders[i] == collider) {
	// 		colliders.erase(colliders.begin() + i);
	// 		break;
	// 	}
	// }
}

void Rigidbody2DComponent::ClearColliders() {
	// colliders.clear();
}

std::vector<ACollider2DComponent*> Rigidbody2DComponent::GetColliders() {
	// return colliders;
}

