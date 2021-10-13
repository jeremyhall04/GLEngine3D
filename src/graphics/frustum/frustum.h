#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../../GLcommon.h"
#include "../../GLcommon_math.h"
#include "../../blocks/block.h"
#include "../../utils/camera.h"

enum class PlaneType 
{
	NEAR,
	FAR,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

struct Plane 
{
	float a, b, c, d;
	glm::vec3 normal;
	void setNormal();
	const glm::vec3 getNormal(bool isNormalized);
	float getDistanceToPoint(const glm::vec4& v);
};

class Frustum
{
private:
	float cam_FOV, cam_Aspect, zFar, zNear;
	PerspectiveCamera* camera;
	Plane leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane;
	Plane m_Planes[6];
	//glm::vec4 leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane;

public:
	Frustum();
	Frustum(PerspectiveCamera* cam);
	~Frustum();

	void updatePlanes();
	bool containsBlock(Block* block);

private:
	bool isVertexIn(glm::vec4 v, PlaneType plane);
};
#endif // !FRUSTUM_H
