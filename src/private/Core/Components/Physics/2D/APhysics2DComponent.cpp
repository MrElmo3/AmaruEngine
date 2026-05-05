#include <Core/Components/Physics/2D/APhysics2DComponent.h>
#include <Core/Scenes/ASceneController.h>
#include <Core/Window.h>
#include <Core/Physics/APhysicsEngine.h>

APhysics2DComponent::APhysics2DComponent(AObject* parent) : IComponent(parent) {}

void APhysics2DComponent::AwakePhysics() {
	PhysicsType scenePhysicsType =  Window::GetInstance().GetActualScene()->scenePhysicsType;
	if(scenePhysicsType != PhysicsType::ENABLE_2D){
		physicsEnabled = false;
	}
}

APhysics2DComponent::~APhysics2DComponent() {
	physicsEnabled = false;
	IComponent::~IComponent();
}

void APhysics2DComponent::Awake() {
	APhysics2DComponent::AwakePhysics();
	isEnabled = physicsEnabled;
}

void APhysics2DComponent::SetEnable(bool isEnable) {
	if(!physicsEnabled) {
		return;
	}
	IComponent::SetEnable(isEnable);
}