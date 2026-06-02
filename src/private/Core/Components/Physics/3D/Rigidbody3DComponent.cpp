#include "Core/Components/Physics/3D/Rigidbody3DComponent.h"

#include "Core/Components/Physics/3D/ACollider3DComponent.h"
#include "Core/Objects/AObject.h"

Rigidbody3DComponent::Rigidbody3DComponent(AObject* parent) : APhysics3DComponent(parent) {
	forceAccumulator = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);
	
	centerOfMass = glm::vec3(0);
	if(ParentHasRigidbody()) enabledPhysics = false;
}

Rigidbody3DComponent::~Rigidbody3DComponent() {
	forceAccumulator = glm::vec3(0);
	acceleration = glm::vec3(0);
	velocity = glm::vec3(0);

	centerOfMass = glm::vec3(0);
	APhysics3DComponent::~APhysics3DComponent();
}

bool Rigidbody3DComponent::ParentHasRigidbody() {
	AObject* aux = parent;
	while(aux != nullptr) {
		if(aux->GetComponent<Rigidbody3DComponent>() != nullptr) return true;
		aux = aux->parent;
	}
	return false;
}

void Rigidbody3DComponent::AddForce(glm::vec3 force) {
	forceAccumulator += force;
}
