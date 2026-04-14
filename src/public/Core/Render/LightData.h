#pragma once
#include <glm/glm.hpp>

enum class LightType {
	DISABLE = 0,
	DIRECTIONAL = 1,
	POINT = 2,
	SPOT = 3,
};

struct LightData {
	alignas(16) glm::vec3 position;
	LightType type;

	alignas(16) glm::vec3 direction;
	float innerRange;

	alignas(16) glm::vec3 color = glm::vec3(1.0);
	float outerRange;

	alignas(16) glm::vec3 ambient = glm::vec3(0.075);
	float constantAtenuation = 1.0f;

	alignas(16) glm::vec3 diffuse = glm::vec3(0.4);
	float linearAtenuation = 0.0f;

	alignas(16) glm::vec3 specular = glm::vec3(1.1);
	float quadraticAtenuation = 0.0f;
};