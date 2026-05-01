#pragma once
#include <Core/Components/IComponent.h>
#include <Core/Physics/APhysicsEngine.h>

class APhysics2DComponent : public IComponent {
protected:
	bool enabledPhysics = true;

public: 
	APhysics2DComponent(AObject* parent);
	virtual ~APhysics2DComponent() override;
	virtual void Awake() override;
	virtual void SetEnable(bool isEnable) override;

protected:
	void AwakePhysics();
};