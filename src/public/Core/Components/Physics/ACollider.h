#pragma once
#include <glm/glm.hpp>
#include <vector>

class ACollider {
public:
	bool isTrigger = false;

protected:
	std::vector<glm::vec3> vertexPoints;
	std::vector<glm::vec3> worldVertexPoints;

public:
	ACollider();
	~ACollider();
	virtual glm::vec3 GetSupportPoint(glm::vec3 direction);

protected:
	virtual void UpdateVertexPoints();
	virtual void UpdateWorldVertexPoints();
	void DrawDebugOutline();
};
