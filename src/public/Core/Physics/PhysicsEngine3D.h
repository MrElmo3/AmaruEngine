#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Physics/APhysicsEngine.h>

class AObject;

/**
 * This class encasulates all the physics calculations for 3D objects.
 */
class PhysicsEngine3D : public APhysicsEngine {

public:
	/**
	 * This class represents a 3D raycast hit.
	 */
	class Raycast3D {
	public:
		bool hit = false;
		float time;
		glm::vec3 position;
	};
	
private:	
	/**
	 * A list of all rigidbodies in the scene.
	 */
// 	static std::vector<Rigidbody3DComponent*> rigidbodies;

	/**
	 * A list of all colliders in the scene.
	 */
// 	static std::vector<ACollider3DComponent*> colliders;

// public:
// 	void Awake(std::vector<AObject*> objects) override;
// 	static void Update();

private:
	/**
	 * @brief Adds the rigidbodies and the colliders inside the object to the physics engine.
	 * @param object The object to search for rigidbodies and colliders.
	 */
// 	static void AddObjectRigidbodyAndCollider(AObject* object);

	/**
	 * @brief Adds a rigidbody to the physics engine.
	 * @param rigidBody The rigidbody to add.
	 */
// 	static void AddRigidbody(Rigidbody3DComponent* rigidBody);
	
	/**
	 * @brief Adds a collider to the physics engine.
	 * @param collider The collider to add.
	 */
// 	static void AddCollider(ACollider3DComponent* collider);

	/**
	 * @brief Searches for the near colliders to the rigidbody.
	 * @param rigidbody The rigidbody to get the near colliders.
	 * @return A list of colliders that are near to the rigidbody.
	 */
// 	static std::vector<ACollider3DComponent*> GetNearColliders(Rigidbody3DComponent* rigidbody);

public:

	/**
	 * @brief Calculates the minimum and maximum points of a box collider.
	 * @param min The minimum point of the box collider.
	 * @param max The maximum point of the box collider.
	 * @param center The center of the box collider.
	 * @param halfSize The half size of the box collider.
	 */
// 	static void CalcMinAndMax(glm::vec3& min, glm::vec3& max, glm::vec3 center, glm::vec3 halfSize);

	/**
	 * @brief Checks if a point intersects a box collider.
	 * @param point The point to check.
	 * @param collider The collider to check.
	 * @return True if the point intersects the collider, false otherwise.
	 */
// 	static bool PointIntersectsBoxCollider(glm::vec3 point, BoxColliderComponent* collider);

	/**
	 * @brief Checks if a box collider intersects another box collider.
	 * @param collider1 The first collider.
	 * @param collider2 The second collider.
	 * @return True if the colliders intersect, false otherwise.
	 */
// 	static bool BoxColliderIntesectsBoxCollider(
// 		BoxColliderComponent* collider1,
// 		BoxColliderComponent* collider2);

	/**
	 * @brief Calculates the penetration vector of two box colliders.
	 * @param collider1 The first collider.
	 * @param collider2 The second collider.
	 * @return The penetration vector of the two colliders if they intersect, otherwise (0, 0, 0).
	 */
// 	static glm::vec3 BoxColliderPenetration(
// 		BoxColliderComponent* collider1,
// 		BoxColliderComponent* collider2);

	/**
	 * @brief Casts a ray in the scene and returns the first collider that intersects the ray.
	 * @param position The position of the ray.
	 * @param direction The direction of the ray.
	 * @param distance The distance of the ray.
	 * @return The first collider that intersects the ray.
	 */
// 	static Raycast3D RaycastBoxCollider(
// 		glm::vec3 position,
// 		glm::vec3 direction,
// 		double distance,
// 		glm::vec3 colliderPosition,
// 		glm::vec3 colliderHalfSize);
};