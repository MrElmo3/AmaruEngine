#pragma once
#include <Core/Objects/AObject.h>

class CameraComponent;

class CameraObject : public AObject{
private:
	CameraComponent* cameraComponent;
	
public:
	CameraObject(const std::string &_name, 
		AObject* _parent = nullptr, 
		ASceneController* _scene = nullptr);
	~CameraObject() override = default;

public:
	CameraComponent* GetCameraComponent() const { return cameraComponent; }
};