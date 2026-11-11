#include "Core/Components/Render/CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Core/Global.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Render.h"
#include "Util/Logger.h"

CameraComponent::CameraComponent(AObject* parent) : IComponent(parent) {
}

CameraComponent::~CameraComponent() {}

void CameraComponent::Awake() {
	if (mainCamera) {
		Render::GetInstance().SetCurrentCamera(this);
	}
}

glm::mat4 CameraComponent::GetViewMatrix() {
	// The view matrix is the inverse of the camera's world transform, so the
	// rotation has to be inverted too (not only the translation). Otherwise the
	// camera turns the opposite way to the object's Forward()/Right().
	view = glm::mat4_cast(glm::inverse(parent->GetWorldRotation()));
	view = glm::translate(view, -1.0f * this->parent->GetWorldPosition());

	return view;
}

glm::mat4 CameraComponent::GetProjectionMatrix() {
	if (orthographic) {
		projection = glm::ortho(
			-1.0f * orthoSize * Global::SCREEN_FORMAT,	orthoSize * Global::SCREEN_FORMAT,	//x size
			-1.0f * orthoSize,							orthoSize,							//y size
			near,										far);								//z size
	}
	else {
		projection = glm::perspective(glm::radians(FOV), Global::SCREEN_FORMAT, near, far);
	}
	return projection;
}