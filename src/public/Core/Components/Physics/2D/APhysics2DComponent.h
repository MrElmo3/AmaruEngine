#pragma once
#include <Core/Components/IComponent.h>
#include <Core/Physics/APhysicsEngine.h>

class APhysics2DComponent : public IComponent {
protected:
	bool enabledPhysics = true;

public: 
	APhysics2DComponent(AObject* parent);
	~APhysics2DComponent();
	void Awake() override;
	void SetEnable(bool isEnable) override;

protected:
	void AwakePhysics();
};