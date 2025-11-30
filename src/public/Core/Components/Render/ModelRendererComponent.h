#pragma once
#include <Core/Components/IComponent.h>

class AMaterial;
class Model;
class ModelRendererComponent : public IComponent {
public:

	Model* model;

	bool enableRender = true;
	AMaterial* material;

public:
	ModelRendererComponent(AObject* parent);
	~ModelRendererComponent() override;
	void LateUpdate() override;

private:
	void Draw();

public:
	ModelRendererComponent* SetModel(Model* model);
};