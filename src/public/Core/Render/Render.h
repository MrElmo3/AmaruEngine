#pragma once
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Util/Singleton.h"

class ColorMaterial;
class AMaterial;
class Model;
class AObject;
class CameraComponent;
class Shader;

/**
 * This class represents the render of the engine
 * Encapsulates all the rendering methods
 */
class Render : public Singleton<Render> {
private:

	CameraComponent* currentCamera;

	AMaterial* currentMaterial;

	std::vector<Shader*> shaders;

	std::map<std::string, unsigned int> textures;

	unsigned int cameraUBO;
	unsigned int lightUBO;

	unsigned int VAOCube;
	unsigned int VBOCubePos;
	unsigned int VBOCubeNorm;
	unsigned int EBOCube;

	unsigned int VAOQuad;
	unsigned int VBOQuadPos;
	unsigned int VBOQuadUv;
	unsigned int VBOQuadNorm;
	unsigned int EBOQuad;

	unsigned int VAOLine;
	unsigned int VBOLine;

public:
	/**
	 * @brief This is a special material used for the gizmos
	 */
	ColorMaterial* gizmosMaterial;

public:
	Render();
	~Render();

private:
	void InitUniformCameraBuffer();
	void InitUniformLightBuffer();
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
	 * @brief Set the camera values to an UBO in the GPU
	 */
	void SetCameraValues();

	/**
	 * @brief Set the light values to an UBO in the GPU
	 */
	void SetLightValues();

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
	 * @brief This method draws a quad in the world
	 * @param modelMatrix The model matrix
	 * @param material The material to use
	 * @param uv A vector with the uv for each vertex of the quad
	 */
	void DrawQuad(
		glm::mat4* modelMatrix,
		AMaterial* material,
		std::vector<glm::vec2>* uv = nullptr
	);

	/**
	 * @brief This method draws a cube in the world
	 * @param modelMatrix The model matrix
	 * @param material The material to use
	 */
	void DrawCube(
		glm::mat4* modelMatrix,
		AMaterial* material,
		std::vector<glm::vec2>* uv = nullptr
	);

	/** 
	 * @brief Draws a custom model loaded and used by 
	 * the RenderModelComponent
	 * @param model The loaded model to 
	 * @param material 
	 * @param modelMatrix 
	 */
	void DrawModel(
		Model* model,
		AMaterial* material,
		glm::mat4* modelMatrix
	);

	/**
	 * @brief Sets a material to use in the renderer if its different from the current material
	 * 
	 * @param material The material to set.
	 */
	void SetCurrentMaterial(AMaterial* material);

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
	glm::mat4 GetTransformMatrix(AObject* object);

	CameraComponent* GetCurrentCamera() const { return currentCamera; }
};

template class Singleton<Render>;