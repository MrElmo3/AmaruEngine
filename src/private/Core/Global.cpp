#include "Core/Global.h"

const float Global::FIXED_DELTA_TIME = 0.016f;

const float Global::SCREEN_FORMAT = (float)Global::SCREEN_WIDTH / (float)Global::SCREEN_HEIGHT;
const float Global::METER = 1;

const float Global::DEBUG_FRAMERATE = 1.0f / 60.0f;

const glm::vec3 Global::FORWARD = glm::vec3(0, 0, -1);
const glm::vec3 Global::RIGHT = glm::vec3(1, 0, 0);
const glm::vec3 Global::UP = glm::vec3(0, 1, 0);

//Phong Lightning constans

const float Global::AMBIENT_LIGHT_STRENGTH = 0.1;