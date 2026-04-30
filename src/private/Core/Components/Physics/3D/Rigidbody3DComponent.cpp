#include "Core/Components/Physics/3D/Rigidbody3DComponent.h"

#include <vector>
// #include "Core/Components/Physics/3D/ACollider3DComponent.h"
// #include "Core/Components/Physics/3D/BoxColliderComponent.h"
#include "Core/Physics/PhysicsEngine3D.h"
#include "Core/Objects/AObject.h"

Rigidbody3DComponent::Rigidbody3DComponent(AObject* parent) : IComponent(parent) {
	velocity = glm::vec3(0.0f);
}

Rigidbody3DComponent::~Rigidbody3DComponent() {
}

void Rigidbody3DComponent::Awake() {
	// for (IComponent* component : parent->components) {
	// 	ACollider3DComponent* collider = dynamic_cast<ACollider3DComponent*>(component);
	// 	if (collider != nullptr) {
	// 		AddCollider(collider);
	// 	}
	// }
}

void Rigidbody3DComponent::End() {
	// colliders.clear();
}

void Rigidbody3DComponent::PhysicsUpdate(
	std::vector<ACollider3DComponent*> nearColliders,
	float fixedDeltaTime) {
	
	// for (ACollider3DComponent* externalCollider : nearColliders) {
	// 	for (ACollider3DComponent* rbCollider: colliders) {
	// 		DetectCollision(externalCollider, rbCollider, fixedDeltaTime);
	// 	}
	// }
	
	// if (velocity != glm::vec3(0))
	// 	parent->SetWorldPosition(parent->GetWorldPosition() + velocity * fixedDeltaTime);
}

void Rigidbody3DComponent::DetectCollision(
	ACollider3DComponent* externalCollider,
	ACollider3DComponent* rbCollider, 
	float fixedDeltaTime) {
	
	// if (externalCollider->isTrigger || rbCollider->isTrigger) return;

	// BoxColliderComponent* externalSquareCollider =
	// 	dynamic_cast<BoxColliderComponent*>(externalCollider);
	// BoxColliderComponent* rbSquareCollider =
	// 	dynamic_cast<BoxColliderComponent*>(rbCollider);

	// if (externalSquareCollider == nullptr || rbSquareCollider == nullptr) return;

	// //simulated collider for the collision detection
	// glm::vec3 sumCenter = externalSquareCollider->GetWorldCenter();
	// glm::vec3 sumHalfSize = (
	// 	externalSquareCollider->GetWorldHalfSize() +
	// 	rbSquareCollider->GetWorldHalfSize()
	// 	);

	// for (int i = 0; i < 3; i++) {
	// 	if (velocity[i] != .0f) {
			
	// 		glm::vec3 direction = glm::vec3(0);
	// 		direction[i] = velocity[i];
			
	// 		PhysicsEngine3D::Raycast3D hit = PhysicsEngine3D::RaycastBoxCollider(
	// 			rbSquareCollider->GetWorldCenter(),
	// 			direction,
	// 			glm::length(velocity) * fixedDeltaTime,
	// 			sumCenter,
	// 			sumHalfSize
	// 		);
	// 		if (hit.hit) {
	// 			parent->SetWorldPosition(hit.position);
	// 			velocity[i] = 0;
	// 		}
	// 	}
	// }
	
	// if (velocity == glm::vec3(0)) {
	// 	if (PhysicsEngine3D::BoxColliderIntesectsBoxCollider(externalSquareCollider, rbSquareCollider)) {
			
	// 		glm::vec3 penetration =  PhysicsEngine3D::BoxColliderPenetration(externalSquareCollider, rbSquareCollider);
			
	// 		parent->SetWorldPosition(parent->GetWorldPosition() + penetration * 1.01f);
	// 	}
	// }
}


void Rigidbody3DComponent::AddCollider(ACollider3DComponent* collider) {
	// colliders.push_back(collider);
}

void Rigidbody3DComponent::RemoveCollider(ACollider3DComponent* collider) {
	// for (int i = 0; i < colliders.size(); i++) {
	// 	if (colliders[i] == collider) {
	// 		colliders.erase(colliders.begin() + i);
	// 		break;
	// 	}
	// }
}

void Rigidbody3DComponent::ClearColliders() {
	// colliders.clear();
}

std::vector<ACollider3DComponent*> Rigidbody3DComponent::GetColliders() {
	// return colliders;
}