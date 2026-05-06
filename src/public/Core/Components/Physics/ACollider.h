#pragma once
#include <glm/glm.hpp>
#include <vector>

class ACollider {
public:
	bool isTrigger = false;

protected:
	std::vector<glm::vec3> vertexPoints;
	std::vector<glm::vec3> worldVertexPoints;

	glm::vec3 aabbMin = glm::vec3(std::numeric_limits<float>::infinity());
	glm::vec3 aabbMax = glm::vec3(-std::numeric_limits<float>::infinity());

public:
	ACollider();
	~ACollider();
	virtual glm::vec3 GetSupportPoint(glm::vec3 direction);
	
	/// @brief Returns the AABB that contains all the world vertex of this collider.
	/// @return the 2 extremes points (min, max)
	virtual std::pair<glm::vec3, glm::vec3> GetAABBContainer() { return {aabbMin, aabbMax}; }

protected:
	void UpdateWorldValues();
	virtual void UpdateLocalVertexPoints();
	virtual void UpdateWorldVertexPoints();
	void DrawDebugOutline();

private:
	void UpdateAABBContainer();
};
