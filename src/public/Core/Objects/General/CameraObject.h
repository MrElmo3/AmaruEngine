#pragma once
#include <Core/Objects/AObject.h>

class CameraComponent;

class CameraObject : public AObject{
public:
	CameraObject(const std::string &_name, 
		AObject* _parent = nullptr, 
		ASceneController* _scene = nullptr);
	~CameraObject() override = default;

private:
	CameraComponent* cameraComponent;

public:
	CameraComponent* GetCameraComponent() const { return cameraComponent; }
};