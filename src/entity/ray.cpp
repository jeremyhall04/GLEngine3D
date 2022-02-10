#include "ray.h"
#include "../world/world.h"
#include "player.h"

void Ray::update(const glm::vec3& startPos, const glm::vec3& dir, float step)
{ 
	endPoint.x = startPos.x + step * dir.x;
	endPoint.y = startPos.y + step * dir.y;
	endPoint.z = startPos.z + step * dir.z;
}

const glm::vec3 Ray::getNormalVec()
{
	return glm::normalize(glm::vec3(endPoint - startPoint));
}

float Ray::getLength() const
{
	return glm::distance(startPoint, endPoint);
}
