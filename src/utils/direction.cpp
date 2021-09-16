#include "direction.h"

const glm::vec3 DIRECTION_VECTORS[6] = {
	glm::vec3( 0,  0,  1), // North
	glm::vec3( 0,  0, -1), // South
	glm::vec3( 1,  0,  0), // East
	glm::vec3(-1,  0,  0), // West
	glm::vec3( 0,  1,  0), // Top
	glm::vec3( 0, -1,  0)  // Bottom
};