#include <Core/Physics/Physics2D/PhysicObject.h>
#include <Core/Physics/Physics2D/Quadtree.h>

namespace Physics2D {
glm::vec2 Quadtree::Node::Center() const {
	return (bottomLeft + topRight) * 0.5f;
}

bool Quadtree::Node::Overlap(PhysicObject* object) const {
	return 
		object->bottomLeft.x < topRight.x 	&& 
		object->bottomLeft.y < topRight.y 	&& 
		object->topRight.x > bottomLeft.x 	&& 
		object->topRight.y > bottomLeft.y;
}

bool Quadtree::Node::Contains(PhysicObject* object) const {
	return 
		object->bottomLeft.x >= bottomLeft.x 	&& 
		object->bottomLeft.y >= bottomLeft.y 	&& 
		object->topRight.x <= topRight.x 		&& 
		object->topRight.y <= topRight.y;
}

void Quadtree::Build(
	const std::vector<PhysicObject*>& objects,
	glm::vec2 worldMin,
	glm::vec2 worldMax) {

	Clear();

	root = std::make_unique<Node>();
	root->bottomLeft = worldMin;
	root->topRight = worldMax;
	root->depth = 0;

	for (auto* obj : objects)
		Insert(root.get(), obj);
}

void Quadtree::Insert(Node* node, PhysicObject* object) {
	if (!node->Overlap(object))
		return;

	if (node->isSplit) {
		for (auto& child : node->children) {
			if (child->Contains(object)) {
				Insert(child.get(), object);
				return;
			}
		}

		node->objects.push_back(object);
		return;
	}

	node->objects.push_back(object);

	if (node->objects.size() > MAX_OBJECTS && node->depth < MAX_DEPTH)
		Split(node);
}

void Quadtree::Split(Node* node) {
	glm::vec2 center = node->Center();
	glm::vec2 bl = node->bottomLeft;
	glm::vec2 tr = node->topRight;
	int nextDepth = node->depth + 1;

	// NW, NE, SW, SE
	node->children[0] = std::make_unique<Node>(Node { { bl.x,		center.y }, { center.x,	tr.y }, nextDepth });
	node->children[1] = std::make_unique<Node>(Node { { center.x,	center.y }, { tr.x,		tr.y }, nextDepth });
	
	node->children[2] = std::make_unique<Node>(Node { { bl.x,		bl.y }, { center.x,	center.y }, nextDepth });
	node->children[3] = std::make_unique<Node>(Node { { center.x,	bl.y }, { tr.x,		center.y }, nextDepth });
	node->isSplit = true;

	auto existing = std::move(node->objects);
	node->objects.clear();

	for (auto* obj : existing) {
		bool fitsInChild = false;

		for (auto& child : node->children) {
			if (child.get()->Contains(obj)) {
				Insert(child.get(), obj);
				fitsInChild = true;
			}
		}
		if (!fitsInChild)
			node->objects.push_back(obj);
	}
}

void Quadtree::GetNearObjects(
	PhysicObject* object,
	std::vector<PhysicObject*>& out) const {
	if (root)
		QueryNode(root.get(), object, out);
}

void Quadtree::QueryNode(
	const Node* node,
	PhysicObject* obj,
	std::vector<PhysicObject*>& out) const {
	if (!node->Overlap(obj))
		return;

	for (auto* candidate : node->objects) {
		if (candidate != obj)
			out.push_back(candidate);
	}

	if (node->isSplit) {
		for (auto& child : node->children)
			QueryNode(child.get(), obj, out);
	}
}

void Quadtree::Clear() {
	if (root.get() != nullptr)
		root.reset();
}
}
