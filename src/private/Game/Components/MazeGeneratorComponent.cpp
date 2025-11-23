#include "Game/Components/MazeGeneratorComponent.h"

#include <iostream>
#include <random>
#include <vector>
#include <glm/glm.hpp>

#include <Util/Utility.h>
#include <Core/Global.h>
#include <Core/Components/Render/RenderCubeComponent.h>
#include <Core/Materials/LitMaterial.h>
#include <Core/Objects/3D/Cube.h>

MazeGeneratorComponent::MazeGeneratorComponent(AObject* parent) : IComponent(parent) {

	size = 5; //Default size of Maze
	seed = 1111; //Default seed
	startID = -1;
	endID = -1;

	do{
		int _size = 0;
		std::cout << "Enter the size of the maze: ";
		std::cin >> _size;
	
		if (_size < 3 || _size % 2 == 0) {
			std::cout << "Invalid size. Size must be an odd number greater than 4." << std::endl;
		}
		SetSize(_size);
	} 
	while (size < 3 || size % 2 == 0);
}

MazeGeneratorComponent::~MazeGeneratorComponent() {
	mazeGraph.~Graph();
}

void MazeGeneratorComponent::Awake() {
	GenerateMaze();
	PrintMaze();
	GenerateObjects();
}

void MazeGeneratorComponent::GenerateMaze() {
	if (!Global::DEBUG) seed = time(0);
	srand(seed);
	std::vector<int> posibleNextNodes;
	std::vector<int> notPosibleNextNodes;

	//Defining the root and the initial posibilities
	mazeGraph = Graph();
	mazeGraph.InsertNode(0);
	posibleNextNodes.push_back(1);
	posibleNextNodes.push_back(size - 2);

	//Creating the maze
	do {
		//Selecting the next node
		int index = rand() % posibleNextNodes.size();
		int newNodeID = posibleNextNodes[index];
		posibleNextNodes.erase(posibleNextNodes.begin() + index);
		mazeGraph.InsertNode(newNodeID);
		if (newNodeID % (size - 2) != 0)		mazeGraph.InsertEdge(newNodeID - 1, newNodeID);
		if ((newNodeID + 1) % (size - 2) != 0)	mazeGraph.InsertEdge(newNodeID + 1, newNodeID);

		mazeGraph.InsertEdge(newNodeID + (size - 2), newNodeID);
		mazeGraph.InsertEdge(newNodeID - (size - 2), newNodeID);

		//checking circuits
		if (mazeGraph.ExistCircuit()) {
			mazeGraph.DeleteNode(newNodeID);
			notPosibleNextNodes.push_back(newNodeID);
			continue;
		}

		//Checking the next posibilities

		//right
		if (!Utility::FindInVector(posibleNextNodes, newNodeID + 1) &&
			!Utility::FindInVector(notPosibleNextNodes, newNodeID + 1) &&
			!mazeGraph.ExistNode(newNodeID + 1) &&
			(newNodeID + 1) % (size - 2) != 0) {

			posibleNextNodes.push_back(newNodeID + 1);
		}

		//left
		if (!Utility::FindInVector(posibleNextNodes, newNodeID - 1) &&
			!Utility::FindInVector(notPosibleNextNodes, newNodeID - 1) &&
			!mazeGraph.ExistNode(newNodeID - 1) &&
			newNodeID % (size - 2) != 0) {

			posibleNextNodes.push_back(newNodeID - 1);
		}

		//down
		if (!Utility::FindInVector(posibleNextNodes, newNodeID + (size - 2)) &&
			!Utility::FindInVector(notPosibleNextNodes, newNodeID + (size - 2)) &&
			(newNodeID + (size - 2)) < ((size - 2) * (size - 2) - 1) &&
			!mazeGraph.ExistNode(newNodeID + (size - 2))) {

			posibleNextNodes.push_back(newNodeID + (size - 2));
		}

		//up
		if (!Utility::FindInVector(posibleNextNodes, newNodeID - (size - 2)) &&
			!Utility::FindInVector(notPosibleNextNodes, newNodeID - (size - 2)) &&
			(newNodeID - (size - 2)) > 0 &&
			!mazeGraph.ExistNode(newNodeID - (size - 2))) {

			posibleNextNodes.push_back(newNodeID - (size - 2));
		}

	} while (posibleNextNodes.size() != 0);

	//Setting the start and end of the maze
	startID = 0;
	endID = mazeGraph.GetLongerLeaf();
}

void MazeGeneratorComponent::GenerateObjects() {
	float objectScale = 1.0f;
	//indicates row
	for (int i = 0; i < size; i++) {
		//indicates column
		for (int j = 0; j < size; j++) {
			int ID = (i - 1) * (size - 2) + (j - 1);
			if (i == 0 || i == size - 1 || j == 0 || j == size - 1) {
				InstantiateWall(
					std::to_string(i) + std::to_string(j),
					glm::vec3(
						-1 * (size)/2 + objectScale * j, 
						0.f,
						+1 * (size)/2 - objectScale * i), 
					glm::vec3(objectScale));
				continue;
			}

			if (mazeGraph.ExistNode(ID)) {
				/*if (ID == startID)		std::cout << "A";
				else if (ID == endID)	std::cout << "B";
				else					std::cout << " ";*/

				continue;
			}

			InstantiateWall(
				std::to_string(i) + std::to_string(j),
				glm::vec3(
					-1 * (size)/2.0f + (objectScale / 2) + objectScale * j,
					0.f,
					+1 * (size)/2.0f - (objectScale / 2) - objectScale * i),
				glm::vec3(objectScale));
		}
	}
}

void MazeGeneratorComponent::PrintMaze() {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i == 0 || i == size - 1 || j == 0 || j == size - 1) {
				std::cout << "#";
				continue;
			}

			int ID = (i - 1) * (size - 2) + (j - 1);
			if (mazeGraph.ExistNode(ID)) {
				if (ID == startID)		std::cout << "A";
				else if (ID == endID)	std::cout << "B";
				else					std::cout << " ";

				continue;
			}
			std::cout << "#";
		}
		std::cout << std::endl;
	}
};

void MazeGeneratorComponent::InstantiateWall(std::string id, glm::vec3 objectPosition, glm::vec3 objectScale) {
	std::string name = "Wall" + id;
	
	Cube* wall = new Cube(name, parent);
	wall->position = objectPosition;
	wall->scale = objectScale * 0.8f;
	
	LitMaterial* _material = dynamic_cast<LitMaterial*>(wall->GetRenderComponent()->material);
	// _material->SetColor(glm::vec3(255, 127, 39) / 255.0f);
	_material->SetDiffuse("Assets/Textures/HL.jpg");
}
