#pragma once
#include <glm/glm.hpp>

class Global {
public:
	static float TIME;

	static const float FIXED_DELTA_TIME;
	/**
	 * Screen width in pixels
	 */
	static const  int SCREEN_WIDTH = 1024;
	/**
	 * Screen height in pixels
	 */
	static const int SCREEN_HEIGHT = 576;
	/**
	 * Screen format (width/height)
	 */
	static const float SCREEN_FORMAT;
	/**
	 * Multiplier of the meter for the engine global space
	 */
	static const  float METER;
	
	static const bool DEBUG = false;
	/**
	 * The framerate of the game in debug mode (Unused)
	 */
	static const float DEBUG_FRAMERATE;

	/**
	 * Forward vector in global space
	 */
	static const glm::vec3 FORWARD;
	/**
	 * Right vector in global space
	 */
	static const glm::vec3 RIGHT;
	/**
	 * Up vector in global space
	 */
	static const glm::vec3 UP;

	//--- Phong lightning constants ---
	static const float AMBIENT_LIGHT_STRENGTH;
	static const float SPECULAR_LIGHT_STRENGTH;
	static const unsigned int SHININESS;

};
