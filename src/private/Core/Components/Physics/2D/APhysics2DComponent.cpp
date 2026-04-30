#include <Core/Components/Physics/2D/APhysics2DComponent.h>
#include <Core/Scenes/ASceneController.h>
#include <Core/Window.h>
#include <Core/Physics/APhysicsEngine.h>

APhysics2DComponent::APhysics2DComponent(AObject* parent) : IComponent(parent) {}

void APhysics2DComponent::AwakePhysics() {
	PhysicsType scenePhysicsType =  Window::GetInstance().GetActualScene()->scenePhysicsType;
	if(scenePhysicsType == PhysicsType::ENABLE_3D){
		enabledPhysics = false;
	}
}

APhysics2DComponent::~APhysics2DComponent() {}

void APhysics2DComponent::Awake() {
	APhysics2DComponent::AwakePhysics();
	isEnabled = enabledPhysics;
}

void APhysics2DComponent::SetEnable(bool isEnable) {
	if(!enabledPhysics) {
		return;
	}
	IComponent::SetEnable(isEnable);
}