#ifndef RAY_H
#define RAY_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"

class World;

struct Ray
{
	float maxRayLength = 5.0f, stepSize = 0.5f;
	glm::vec3 startPoint, endPoint;
	void update(const glm::vec3& startPos, const glm::vec3& dir, float step);
	const glm::vec3 getNormalVec();
	float getLength() const;
};

#endif // !RAY_H
