#pragma once
#include <Core/Components/IComponent.h>

class AMaterial;

class ModelRendererComponent : public IComponent {
public:
	
	bool enableRender = true;
	AMaterial* material;

public:
	ModelRendererComponent(AObject* parent);
	~ModelRendererComponent() override;
	void LateUpdate() override;

private:
	void Draw();
};