#include <Core/Objects/3D/Cube.h>

#include <Core/Components/Physics/3D/BoxColliderComponent.h>
#include <Core/Components/Render/RenderCubeComponent.h>

Cube::Cube(const std::string &_name, AObject* _parent, ASceneController* _scene)
	: AObject(_name, _parent, _scene) {
	renderComponent = dynamic_cast<RenderCubeComponent*>(
		AddComponent(new RenderCubeComponent(this))
	);

	colliderComponent = dynamic_cast<BoxColliderComponent*>(
		AddComponent(new BoxColliderComponent(this))
	);
}
