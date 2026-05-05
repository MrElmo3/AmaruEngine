#pragma once

class AObject;

class IComponent {
friend class AObject;

protected:
	bool isEnabled = true;
	bool isDirty = true;
public:
	
	AObject* parent;
	IComponent() : parent(nullptr) {}
	IComponent(AObject* parent) { this->parent = parent; }
	virtual ~IComponent(){}
	virtual void Awake(){}
	virtual void Start() {}
	virtual void FixedUpdate() {}
	virtual void Update(double deltaTime) {}
	virtual void LateUpdate() { isDirty = false; }
	virtual void End() {}
	virtual void SetEnable(bool isEnable) { this->isEnabled = isEnable; }
	bool IsEnabled() { return isEnabled; }
	void MarkDirty() { isDirty = true; };
};
