#include <Core/Components/Physics/3D/APhysics3DComponent.h>
#include <Core/Scenes/ASceneController.h>
#include <Core/Window.h>
#include <Core/Physics/APhysicsEngine.h>
#include <Core/Objects/AObject.h>

APhysics3DComponent::APhysics3DComponent(AObject* parent) : IComponent(parent) {}

APhysics3DComponent::~APhysics3DComponent() {
	enabledPhysics = false;
	IComponent::~IComponent();
}

void APhysics3DComponent::Awake() {
	APhysics3DComponent::AwakePhysics();
	isEnabled = enabledPhysics;
	if(isEnabled){
		parent->scene->physicsEngine->RegisterObject(parent);
	}
}

void APhysics3DComponent::AwakePhysics() {
	PhysicsType scenePhysicsType =  parent->scene->scenePhysicsType;
	if(scenePhysicsType != PhysicsType::ENABLE_3D){
		enabledPhysics = false;
	}
}

void APhysics3DComponent::SetEnable(bool isEnable) {
	if(!enabledPhysics) {
		return;
	}
	IComponent::SetEnable(isEnable);
}