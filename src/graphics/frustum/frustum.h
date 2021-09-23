#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "../../GLcommon.h"
#include "../../GLcommon_math.h"
#include "../../blocks/chunk.h"
#include "../../utils/camera.h"

class Frustrum;

enum class PlaneType {
	NEAR,
	FAR,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM
};

struct Plane {
	float a, b, c, d;
};

class Frustum
{
private:
	float cam_FOV, cam_Aspect, zFar, zNear;
	PerspectiveCamera* camera;
	Plane leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane;
	//glm::vec4 leftPlane, rightPlane, topPlane, bottomPlane, nearPlane, farPlane;

public:
	Frustum(PerspectiveCamera* cam);
	~Frustum();

	void updatePlanes();
	void containsBlock(Block* block);

private:
	bool isVertexIn(glm::vec4 v, PlaneType plane);
};
#endif // !FRUSTUM_H
