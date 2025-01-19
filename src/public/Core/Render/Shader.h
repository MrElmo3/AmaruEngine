#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>


/**
 * This class represents a shader program
 */
class Shader {
public:
	/**
	 * The ID of the shader program
	 */
	unsigned int ID;

private:
	std::string vertexPath;
	std::string fragmentPath;

public:
	/**
	 * @brief Creates a shader program
	 * @param vertexPath The path of the vertex shader
	 * @param fragmentPath The path of the fragment shader
	 */
	Shader(const std::string vertexPath, const std::string fragmentPath);

	/**
	 * @brief Use the shader program
	 */
	void Use() const {
		glUseProgram(ID);
	}

	/**
	 * @brief Set a boolean uniform in the shader
	 * @param name The name of the uniform
	 * @param value The value to set the uniform
	 */
	void SetBool(const std::string &name, bool value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	/**
	 * @brief Set an integer uniform in the shader
	 * @param name The name of the uniform
	 * @param value The value to set the uniform
	 */
	void SetInt(const std::string &name, int value) const{
    	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

	/**
	 * @brief Set a float uniform in the shader
	 * @param name The name of the uniform
	 * @param value The value to set the uniform
	 */
	void SetFloat(const std::string &name, float value) const{
    	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

	/**
	 * @brief Set a vector2 uniform in the shader
	 * @param name The name of the uniform
	 * @param value The value to set the uniform
	 */
	void SetVector3(const std::string &name, glm::vec3 value) {
		int location = glGetUniformLocation(ID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	/**
	 * @brief Set a vector3 uniform in the shader
	 * @param name The name of the uniform
	 * @param value The value to set the uniform
	 */
	void SetMatrix4(const std::string &name, glm::mat4 value) {
		int location = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

public:
	/**
	 * @brief Compare if 2 shaders uses the same files
	 * @param vertexPath The path of the vertex shader
	 * @param fragmentPath The path of the fragment shader
	 * @return True if the shaders uses the same files, false otherwise
	 */
	bool CompareFiles(const std::string vertexPath, const std::string fragmentPath) const {
		return this->vertexPath == vertexPath && this->fragmentPath == fragmentPath;
	}

	/**
	 * @brief Compare if 2 shaders have the same ID
	 * @param other The shader to compare
	 * @return	True if the shaders have the same ID, false otherwise
	 */
	bool operator==(const Shader& other) const {
		return this->ID == other.ID;
	}
};
