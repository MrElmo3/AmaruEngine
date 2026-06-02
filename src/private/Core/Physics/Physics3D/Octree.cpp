#include <Core/Physics/Physics3D/Octree.h>
#include <Core/Physics/Physics3D/PhysicObject.h>

namespace Physics3D {
glm::vec3 Octree::Node::Center() const{
	return (bottomLeft + topRight) * 0.5f;
}

bool Octree::Node::Overlap(PhysicObject* object) const {
	return 
		object->bottomLeft.x < topRight.x 	&& 
		object->bottomLeft.y < topRight.y 	&& 
		object->bottomLeft.z < topRight.z	&&
		object->topRight.x > bottomLeft.x 	&& 
		object->topRight.y > bottomLeft.y	&&
		object->topRight.z > bottomLeft.z;
}

bool Octree::Node::Contains(PhysicObject* object) const {
	return 
		object->bottomLeft.x >= bottomLeft.x 	&& 
		object->bottomLeft.y >= bottomLeft.y 	&& 
		object->bottomLeft.z >= bottomLeft.z 	&&
		object->topRight.x <= topRight.x 		&& 
		object->topRight.y <= topRight.y		&&
		object->topRight.z <= topRight.z;
}

void Octree::Build(
	const std::vector<PhysicObject*>& objects,
	glm::vec3 worldMin,
	glm::vec3 worldMax) {

	Clear();

	root = std::make_unique<Node>();
	root->bottomLeft = worldMin;
	root->topRight = worldMax;
	root->depth = 0;

	for (auto* obj : objects)
		Insert(root.get(), obj);
}

void Octree::Insert(Node* node, PhysicObject* object) {
	if(!node->Overlap(object))
		return;
	
	if(node->isSplit) {
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

	if(node->objects.size() > MAX_OBJECTS && node->depth < MAX_DEPTH) {
		Split(node);
	}
}

void Octree::Split (Node* node) {
	glm::vec3 center = node->Center();
	glm::vec3 bl = node->bottomLeft;
	glm::vec3 tr = node->topRight;
	int nextDepth = node->depth + 1;

	node->children[0] = std::make_unique<Node>(Node { { bl.x, 		center.y , 	center.z }, { center.x,	tr.y,		tr.z }, nextDepth });
	node->children[1] = std::make_unique<Node>(Node { { center.x, 	center.y, 	center.z }, { tr.x,		tr.y, 		tr.z }, nextDepth });
	node->children[2] = std::make_unique<Node>(Node { { bl.x, 		bl.y, 		center.z }, { center.x,	center.y,	tr.z }, nextDepth });
	node->children[3] = std::make_unique<Node>(Node { { center.x, 	bl.y, 		center.z }, { tr.x,		center.y,	tr.z }, nextDepth });
	
	node->children[4] = std::make_unique<Node>(Node { { bl.x, 		center.y , 	bl.z }, { center.x,	tr.y,		center.z }, nextDepth });
	node->children[5] = std::make_unique<Node>(Node { { center.x, 	center.y, 	bl.z }, { tr.x,		tr.y, 		center.z }, nextDepth });
	node->children[6] = std::make_unique<Node>(Node { { bl.x, 		bl.y, 		bl.z }, { center.x,	center.y,	center.z }, nextDepth });
	node->children[7] = std::make_unique<Node>(Node { { center.x, 	bl.y, 		bl.z }, { tr.x,		center.y,	center.z }, nextDepth });

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

void Octree::GetNearObjects(
	PhysicObject* object,
	std::vector<PhysicObject*>& out) const {
	if (root)
		QueryNode(root.get(), object, out);
}

void Octree::QueryNode(
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

void Octree::Clear() {
	if (root.get() != nullptr)
		root.reset();
}
}