#pragma once
#include "Core/Scenes/ASceneController.h"
#include <glm/glm.hpp>

class EmptyObject;

/**
 * @brief 3D version of CollisionScene: a closed box of static box colliders
 * where pressing Z spawns a burst of spheres with random 3D velocities.
 */
class Collision3DScene : public ASceneController {
private:
	EmptyObject* cameraObject;

public:
	Collision3DScene();
	void Update(double deltaTime) override;

private:
	void CreateWall(glm::vec3 position, glm::vec3 scale);
	void CreateSpheres();

	bool spheresCreated = false;
};
