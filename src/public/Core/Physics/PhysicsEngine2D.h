#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <Core/Physics/APhysicsEngine.h>

class AObject;
class ACollider2DComponent;
class Rigidbody2DComponent;

/**
 * This class encasulates all the physics calculations for 2D objects.
 */
class PhysicsEngine2D : public APhysicsEngine {
	
public:
	/**
	 * This class represents a 2D raycast hit.
	 */
	class Raycast2D {
	public:
		bool hit = false;
		float time;
		glm::vec2 position;
	};
	
private:
	/**
	 * A list of all rigidbodies in the scene.
	 */
	// static std::vector<Rigidbody2DComponent*> rigidbodies;
	/**
	 * A list of all colliders in the scene.
	 */
	// static std::vector<ACollider2DComponent*> colliders;

public:
	// void Awake(std::vector<AObject*> objects) override;
	// static void Update();

private:
	/**
	 * @brief Adds the rigidbodies and the colliders inside the object to the physics engine.
	 * @param object The object to search for rigidbodies and colliders.
	 */
	// static void AddObjectRigidbodyAndCollider(AObject* object);
	
	/**
	 * @brief Adds a rigidbody to the physics engine.
	 * @param rigidBody The rigidbody to add.
	 */
	// static void AddRigidbody(Rigidbody2DComponent* rigidBody);

	/**
	 * @brief Adds a collider to the physics engine.
	 * @param collider The collider to add.
	 */
	// static void AddCollider(ACollider2DComponent* collider);

	/**
	 * @brief Searches for the near colliders to the rigidbody.
	 * @param rigidbody The rigidbody to get the near colliders.
	 * @return A list of colliders that are near to the rigidbody.
	 */
	// static std::vector<ACollider2DComponent*> GetNearColliders(Rigidbody2DComponent* rigidbody);
	
public:
	/**
	 * @brief Calculate the min and max vertices positions of a square collider
	 * and stores them in the min and max vectors.
	 * @param min A reference to the min vector.
	 * @param max A reference to the max vector.
	 * @param center The center of the quad.
	 * @param halfSize The half size of the quad.
	 */
	// static void CalcMinAndMax(glm::vec2& min, glm::vec2& max, glm::vec2 center, glm::vec2 halfSize);

	/**
	 * @brief Checks if a point intersects a square collider.
	 * @param point The point to check.
	 * @param collider The collider to check.
	 * @return True if the point intersects the collider, false otherwise.
	 */
	// static bool PointIntersectsSquareCollider(glm::vec2 point, SquareColliderComponent* collider);

	/**
	 * @brief Checks if a square collider intersects another square collider.
	 * @param collider1 The first collider to check.
	 * @param collider2 The second collider to check.
	 * @return True if the colliders intersect, false otherwise.
	 */
	// static bool SquareColliderIntesectsSquareCollider(
	// 	SquareColliderComponent* collider1,
	// 	SquareColliderComponent* collider2);

	/**
	 * @brief Calculates the penetration vector of two square colliders.
	 * @param collider1 The first collider to check.
	 * @param collider2 The second collider to check.
	 * @return The penetration vector of the colliders if they intersect, otherwise (0, 0).
	 */
	// static glm::vec2 SquareColliderPenetration(
	// 	SquareColliderComponent* collider1,
	// 	SquareColliderComponent* collider2);

	/**
	 * @brief Checks if a raycast intersects a square collider.
	 * @param position The start position of the raycast.
	 * @param direction The direction of the raycast.
	 * @param distance The distance of the raycast.
	 * @param colliderPosition The position of the collider to check the collision.
	 * @param colliderHalfSize The half size of the collider to check the collision.
	 * @return A Raycast2D object with the information of the raycast.
	 */
	// static Raycast2D RaycastSquareCollider(
	// 	glm::vec2 position,
	// 	glm::vec2 direction,
	// 	double distance,
	// 	glm::vec2 colliderPosition,
	// 	glm::vec2 colliderHalfSize);
};
