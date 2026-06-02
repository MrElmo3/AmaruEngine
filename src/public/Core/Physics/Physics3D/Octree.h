#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Physics3D {

class PhysicObject;

class Octree {
public:
	static constexpr int MAX_OBJECTS = 4;
	static constexpr int MAX_DEPTH = 8;

private:
	struct Node {
		glm::vec3 bottomLeft;
		glm::vec3 topRight;
		int depth;

		std::vector<PhysicObject*> objects;
		std::unique_ptr<Node> children[8];
		bool isSplit = false;

		/// @brief Gets the world center of the node.
		/// @return The center  of the node.
		glm::vec3 Center() const;

		/// @brief Checks if the node intersect with a given physic object.
		/// @param obj The object to check.
		/// @return True if the node intersects.
		bool Overlap(PhysicObject* obj) const;

		bool Contains(PhysicObject* obj) const;
	};

	std::unique_ptr<Node> root;

	/// @brief
	///		Inserts an object into a given node if the node contains the object.
	/// 	If no, do nothing.
	/// @param node The node where to insert.
	/// @param obj 	The object to insert.
	void Insert(Node* node, PhysicObject* obj);

	/// @brief Splits the a given node.
	/// @param node The node to split.
	void Split(Node* node);

	/// @brief Auxiliar function of GetNearObjects to recursive searchs in the tree.
	/// @param node the Node to analyze.
	/// @param object The object of GetNearObjects.
	/// @param out The out vector that contains all of the candidates.
	void QueryNode(
		const Node* node,
		PhysicObject* object,
		std::vector<PhysicObject*>& out) const;

public:
	/// @brief Builds the quadtree using an object list withing certain limits
	/// @param objects The reference of the objects.
	/// @param worldMin The bottom left point of the tree.
	/// @param worldMax Te  top right point of the tree.
	void Build(
		const std::vector<PhysicObject*>& objects,
		glm::vec3 worldMin,
		glm::vec3 worldMax);

	/// @brief Gets the near objects arround a given object.
	/// @param object The object to check arround.
	/// @param out A vector of the objects arround the given object.
	void GetNearObjects(
		PhysicObject* object,
		std::vector<PhysicObject*>& out) const;

	/// @brief Clears the tree.
	void Clear();
};
}
