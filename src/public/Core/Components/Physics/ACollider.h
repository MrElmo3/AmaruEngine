#pragma once
#include <glm/glm.hpp>
#include <vector>

class ACollider {
public:
	bool isTrigger = false;

protected:
	std::vector<glm::vec3> vertexPoints;

	ACollider();
	~ACollider();
	virtual glm::vec3 GetSupportPoint(glm::vec3 direction);

protected:
	virtual void UpdateVertexPoints();
	void DrawDebugOutline();
};
