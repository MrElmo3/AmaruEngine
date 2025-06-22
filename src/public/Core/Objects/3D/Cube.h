#pragma once
#include <Core/Objects/AObject.h>

class BoxColliderComponent;
class RenderCubeComponent;

class Cube : public AObject {
public:
	Cube(const std::string &_name,
		AObject* _parent = nullptr,
		ASceneController* _scene = nullptr);

protected:
	RenderCubeComponent* renderComponent;
	BoxColliderComponent* colliderComponent;

public:
	RenderCubeComponent* GetRenderComponent() const { return renderComponent; }
	BoxColliderComponent* GetBoxColliderComponent() const { return colliderComponent; }
};
