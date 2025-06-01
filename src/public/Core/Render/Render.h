#pragma once
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Util/Singleton.h"

class ColorMaterial;
class AMaterial;
class CameraComponent;
class Shader;

/**
 * This class represents the render of the engine
 * Encapsulates all of the rendering methods
 */
class Render : public Singleton<Render> {
private:

	CameraComponent* currentCamera;
	
	std::vector<Shader*> shaders;

	std::map<std::string, unsigned int> textures;

	unsigned int VAO_cube;
	unsigned int VBO_cube;
	unsigned int EBO_cube;
	
	unsigned int VAO_quad;
	unsigned int VBO_quad;
	unsigned int EBO_quad;
	
	unsigned int VAO_line;
	unsigned int VBO_line;

public:
	/**
	 * @brief This is a special material used for the gizmos
	 */
	ColorMaterial* gizmosMaterial;

public:
	Render();
	~Render();

private:
	void InitQuad();
	void InitLine();
	void InitCube();

public:
	
	/**
	 * @brief This method creates a shader program
	 * @param vertexPath The path of the vertex shader
	 * @param fragmentPath The path of the fragment shader
	 * @return An object of the Shader class representing the shader program
	 */
	Shader* CreateShader(
		const std::string vertexPath, 
		const std::string fragmentPath
	);

	/**
	 * @brief Generates a texture from a file
	 * @param texturePath The path of the texture
	 * @return Returns the ID of the generated texture
	 */
	unsigned int GenerateTexture(const std::string texturePath);

	/**
	 * @brief This method draws a quad in the world
	 * @param model The model matrix
	 * @param material The material to use
	 */
	void DrawQuad(glm::mat4 model, AMaterial* material);

	/**
	 * @brief This method draws a line segment in the world using the 
	 * gizmos material
	 * @param start The start point of the line
	 * @param end The end point of the line
	 * @param color The color of the line
	 */
	void DrawLineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color);

	/**
	 * @brief This method draws a quad line in the world
	 * @param center The center of the quad
	 * @param size The size of the quad
	 * @param color The color of the lines
	 */
	void DrawQuadLine(glm::vec2 center, glm::vec2 size, glm::vec3 color);

	/**
	 * @brief This method draws a cube in the world
	 * @param model The model matrix
	 * @param material The material to use
	 */
	void DrawCube(glm::mat4 model, AMaterial* material);

	/**
	 * @brief This method sets the current camera
	 * @param camera The camera to set as the current camera
	 */
	void SetCurrentCamera(CameraComponent* camera);

	/**
	 * @brief Encapsulates the logic of calculation of the 
	 * model matrix of an object
	 * @param object the object to calculate the matrix
	 */
	glm::mat4 GetModelMatrix(AObject* object);
	
	CameraComponent* GetCurrentCamera() const { return currentCamera; }
};

template class Singleton<Render>;