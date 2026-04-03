#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>

class Utility {
public:
	/**
	 * @brief This method checks if a value is in a vector
	 * @param vec The vector to search in
	 * @param value The value to search
	 * @return True if the value is in the vector, false otherwise
	 */
	static inline bool FindInVector(std::vector<int> vec, int value) {
		for (int i = 0; i < vec.size(); i++) {
			if (vec[i] == value) {
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief Calculate the angle between two vectors
	 * @param a Vector 3 A
	 * @param b Vector 3 B
	 * @return  The cosine of the angle between the two vectors
	 */
	static inline float AngleBetweenVectors(glm::vec3 a, glm::vec3 b) {
		float dot = glm::dot(a, b);
		float len = glm::length(a) * glm::length(b);
		float result = dot / len;
		return result;
	}

	/// @brief Converts a 4x4 Assimp matrix to a 4x4 GLM matrix.
	/// @param from The Assimp matrix.
	/// @return The 4x4 GLM matrix.
	static inline glm::mat4 ConvertMatrix4x4AssimpToGLM(const aiMatrix4x4& from) {
		return glm::mat4(
			from.a1, from.b1, from.c1, from.d1,
			from.a2, from.b2, from.c2, from.d2,
			from.a3, from.b3, from.c3, from.d3,
			from.a4, from.b4, from.c4, from.d4 
		);
	}
};
