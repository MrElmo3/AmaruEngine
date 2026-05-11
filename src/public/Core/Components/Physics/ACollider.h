#pragma once
#include <glm/glm.hpp>
#include <vector>

class ACollider {
public:
	bool isTrigger = false;

	glm::vec3 debugColor;

protected:
	/// @brief The vertex points of the collider in local space.
	std::vector<glm::vec3> vertexPoints;

	/// @brief The vertex points of the collider in world space.
	std::vector<glm::vec3> worldVertexPoints;

	/// @brief The center of the collider in world space.
	glm::vec3 center;

	/// @brief The min point of the min AABB that contains this collider.
	glm::vec3 aabbMin = glm::vec3(FLT_MAX);

	/// @brief the max point of the min AABB that contains this collider.
	glm::vec3 aabbMax = glm::vec3(-FLT_MAX);

public:
	ACollider();
	~ACollider();
	virtual glm::vec3 GetSupportPoint(glm::vec3 direction);

	/// @brief Returns the AABB that contains all the world vertex of this collider.
	/// @return the 2 extremes points (min, max)
	virtual std::pair<glm::vec3, glm::vec3> GetAABBContainer() { return { aabbMin, aabbMax }; }

	/// @brief Gets the center of the collider.
	/// @return The center of the collider.
	glm::vec3 GetCenter() const { return center; }

protected:
	void UpdateWorldValues();
	virtual void UpdateLocalVertexPoints();
	virtual void UpdateWorldVertexPoints();
	void UpdateCenterValue();
	void DrawDebugOutline();

private:
	void UpdateAABBContainer();
};
