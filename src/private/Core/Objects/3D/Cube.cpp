#include <Core/Objects/3D/Cube.h>

#include <Core/Components/Physics/3D/BoxColliderComponent.h>
#include <Core/Components/Render/RenderCubeComponent.h>

Cube::Cube(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {

	renderComponent = AddComponent<RenderCubeComponent>();
	colliderComponent = AddComponent<BoxColliderComponent>();
}

void Cube::Update(double deltaTime) {
	AObject::Update(deltaTime);
	// this->RotateEuler(glm::vec3(0.0, 30.0, 0.0) * (float)deltaTime);
}

