#pragma once
#include "Core/Components/IComponent.h"
#include <vector>
#include <glm/glm.hpp>

class AMaterial;

class RenderCubeComponent : public IComponent {
public:
	bool enableRender = true;
	AMaterial* material;
	
public:
	explicit RenderCubeComponent(AObject* parent);
	~RenderCubeComponent() override;
	void LateUpdate() override;

private:
	void Draw();
};
