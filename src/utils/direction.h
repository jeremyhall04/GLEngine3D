#ifndef DIRECTION_H
#define DIRECTION_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"

class Renderable3D;

enum Direction {
	D_NORTH	= 0,
	D_SOUTH	= 1,
	D_EAST	= 2,
	D_WEST	= 3,
	D_UP	= 4,
	D_DOWN	= 5
};

extern const glm::vec3 DIRECTION_VECTORS[6];

#endif // !DIRECTION_H
