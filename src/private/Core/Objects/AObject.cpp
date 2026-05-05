#include "Core/Objects/AObject.h"

#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <Util/Logger.h>
#include <Core/Components/IComponent.h>
#include "Core/Scenes/ASceneController.h"
#include "Core/Window.h"

AObject::AObject(const std::string &_name, AObject* _parent, ASceneController* _scene) {

	if (!((_parent != nullptr) ^ (_scene != nullptr))){
		Logger::Error("The object " + _name + " needs either a parent or a scene specification");
		return;
	}

	this->name = _name;

	if (_parent) {
		this->parent = _parent;
		parent->AddChild(this);
	}

	if (_scene) {
		this->scene = _scene;
		scene->AddObject(this);
	}
}

AObject::~AObject() = default;

void AObject::Awake() {
	for (auto element : components) {
		if (!element->IsEnabled()) continue;
		element->Awake();
	}

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->Awake();
	}
}

void AObject::Start() {
	for (auto element : components) {
		if (!element->IsEnabled()) continue;
		element->Start();
	}

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->Start();
	}
}

void AObject::FixedUpdate() {

	for (auto element : components) {
		if (!element->IsEnabled()) continue;
		element->FixedUpdate();
	}

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->FixedUpdate();
	}
}


void AObject::Update(const double deltaTime) {

	for (auto element : components) {
		if (!element->IsEnabled()) continue;
		element->Update(deltaTime);
	}

	UpdateWorldValues();

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->Update(deltaTime);
	}
}

void AObject::LateUpdate() {
	UpdateTransform();

	for (auto element : components) {
		if (!element->IsEnabled()) continue;
		element->LateUpdate();
	}

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->LateUpdate();
	}

	isDirty = false;
}

void AObject::End() {
	for (auto element : components) {
		delete element;
	}

	for (auto element : children) {
		element->End();
		delete element;
	}
}

void AObject::AddChild(AObject* child) {
	children.push_back(child);
}

void AObject::Translate(glm::vec3 delta) {
	position += delta;
	MarkDirty();
}

void AObject::Rotate(glm::quat rotation) {
	rotation = glm::normalize(rotation);
	this->rotation = this->rotation * rotation;
	MarkDirty();
}

void AObject::RotateEuler(glm::vec3 rotation) {
	if(rotation == glm::vec3(0)) return;
	rotation = glm::radians(rotation);
	Rotate(glm::quat(rotation));
}

void AObject::SetPosition(glm::vec3 position) {
	this->position = position;
	MarkDirty();
}

void AObject::SetRotation(glm::quat rotation) {
	rotation = glm::normalize(rotation);
	this->rotation = rotation;
	MarkDirty();
}

void AObject::SetScale(glm::vec3 scale) {
	this->scale = scale;
	MarkDirty();
}

void AObject::SetWorldPosition(const glm::vec3 position) {
	if (parent != nullptr) {
		this->position = glm::vec3(
			(position.x - parent->worldPosition.x) / parent->worldScale.x,
			(position.y - parent->worldPosition.y) / parent->worldScale.y,
			(position.z - parent->worldPosition.z) / parent->worldScale.z
		);
	}
	else {
		this->position = position;
	}
	MarkDirty();
}

void AObject::SetWorldRotation(glm::quat rotation) {
	// rotation = glm::normalize(rotation);
	// this->rotationQuat = rotation;
	MarkDirty();
}

void AObject::SetWorldScale(const glm::vec3 scale) {
	if (parent != nullptr) {
		this->scale = glm::vec3(
			scale.x / parent->worldScale.x,
			scale.y / parent->worldScale.y,
			scale.z / parent->worldScale.z
		);
	}
	else {
		this->scale = scale;
	}
	MarkDirty();
}

void AObject::UpdateWorldValues() {
	if(!isDirty) return;
	if (parent != nullptr) {

		worldScale    = parent->worldScale * scale;
		worldPosition = parent->worldPosition + parent->worldRotation * (parent->worldScale * position);
		worldRotation = parent->worldRotation * rotation;
	}
	else {
		this->worldScale = scale;
		this->worldPosition = position;
		this->worldRotation = rotation;
	}
}

void AObject::UpdateTransform() {
	if(!isDirty) return;

	glm::mat4 matrix = glm::mat4(1);

	glm::quat rotation = GetWorldRotation();
	glm::vec3 vectorRot = glm::vec3(rotation.x, rotation.y, rotation.z);
	float angle = 2 * glm::acos(rotation.w);

	matrix = glm::translate(matrix, GetWorldPosition());
	if (vectorRot != glm::vec3(0))
		matrix = glm::rotate(matrix, angle, vectorRot);
	matrix = glm::scale(matrix, GetWorldScale());

	worldTransform = matrix;
}

void AObject::MarkDirty() {
	if (isDirty) return;  // already dirty, stop propagating
    isDirty = true;

	for(auto& component : components) {
		component->MarkDirty();
	}

    for (auto& child : children)
        child->MarkDirty();
}

AObject* AObject::FindObjectByName(const std::string& name) {
	
	ASceneController* scene = Window::GetInstance().GetActualScene();

	for (AObject* object : scene->objects) {
		if (object->name == name) {
			return object;
		}
	}
	return nullptr;
}


