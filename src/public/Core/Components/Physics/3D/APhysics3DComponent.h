#pragma once
#include <Core/Components/IComponent.h>
#include <Core/Physics/APhysicsEngine.h>

class APhysics3DComponent : public IComponent {
protected:
	bool enabledPhysics = true;

public: 
	APhysics3DComponent(AObject* parent);
	virtual ~APhysics3DComponent() override;
	virtual void Awake() override;
	virtual void SetEnable(bool isEnable) override;

	bool PhysicsEnabled() { return enabledPhysics; }

protected:
	void AwakePhysics();
};