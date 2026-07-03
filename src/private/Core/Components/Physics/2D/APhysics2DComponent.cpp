#include <Core/Components/Physics/2D/APhysics2DComponent.h>
#include <Core/Scenes/ASceneController.h>
#include <Core/Window.h>
#include <Core/Objects/AObject.h>
#include <Core/Physics/APhysicsEngine.h>

APhysics2DComponent::APhysics2DComponent(AObject* parent) : IComponent(parent) {}

APhysics2DComponent::~APhysics2DComponent() {
	enabledPhysics = false;
	IComponent::~IComponent();
}

void APhysics2DComponent::Awake() {
	APhysics2DComponent::AwakePhysics();
	isEnabled = enabledPhysics;
	if(isEnabled){
		parent->scene->physicsEngine->RegisterObject(parent);
	}

}

void APhysics2DComponent::AwakePhysics() {
	PhysicsType scenePhysicsType =  parent->scene->scenePhysicsType;
	if(scenePhysicsType != PhysicsType::ENABLE_2D){
		enabledPhysics = false;
	}
}

void APhysics2DComponent::SetEnable(bool isEnable) {
	if(!enabledPhysics) {
		return;
	}
	IComponent::SetEnable(isEnable);
}