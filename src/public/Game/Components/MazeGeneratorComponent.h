#pragma once
#include <string>
#include <glm/glm.hpp>

#include "Core/Components/IComponent.h"
#include "Util/Graph.h"	

class MazeGeneratorComponent : public IComponent {
private:
	Graph mazeGraph;
	int size;
	long seed;
	int startID;
	int endID;

public:
	MazeGeneratorComponent(AObject* parent);
	~MazeGeneratorComponent() override;
	void Awake() override;

private:
	void GenerateMaze();
	void GenerateObjects();
	void PrintMaze();

	void InstantiateWall(std::string id, glm::vec3 objectPosition, glm::vec3 objectSize);

public:
	int GetSize() const { return size; }
	void SetSize(int size) { this->size = size; }
	long GetSeed() const { return seed; }
	void SetSeed(long seed) { this->seed = seed; }

	/// @brief World position of the center of a maze cell (local to the maze object).
	glm::vec3 GetCellPosition(int row, int column) const;

	/// @brief World position of the start cell (marked as A in the console).
	glm::vec3 GetStartPosition() const;

	/// @brief World position of the end cell (marked as B in the console): the dead end
	/// farthest from the start. Only valid after the maze is generated (Awake).
	glm::vec3 GetEndPosition() const;
};