#include "Core/Objects/General/CameraObject.h"
#include <Core/Components/Render/CameraComponent.h>

CameraObject::CameraObject(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {
	
	cameraComponent = dynamic_cast<CameraComponent*>(
		AddComponent(new CameraComponent(this))
	);
	cameraComponent->orthographic = false;
	cameraComponent->mainCamera = false;
	cameraComponent->FOV = 60.0f;
}