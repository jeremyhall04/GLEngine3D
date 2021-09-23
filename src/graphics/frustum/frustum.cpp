#include "frustum.h"

Frustum::Frustum(PerspectiveCamera* cam)
	: camera(cam)
{
	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	updatePlanes();
}

void Frustum::updatePlanes()
{
	glm::mat4 proj = camera->getProjectionMatrix();

	/*leftPlane =		glm::vec4(proj[3][0] + proj[0][0], proj[3][1] + proj[0][1], proj[3][2] + proj[0][2], proj[3][3] + proj[0][3]);
	rightPlane =	glm::vec4(proj[3][0] - proj[0][0], proj[3][1] - proj[0][1], proj[3][2] - proj[0][2], proj[3][3] - proj[0][3]);
	bottomPlane =	glm::vec4(proj[3][0] + proj[1][0], proj[3][1] + proj[1][1], proj[3][2] + proj[1][2], proj[3][3] + proj[1][3]);
	topPlane =		glm::vec4(proj[3][0] - proj[1][0], proj[3][1] - proj[1][1], proj[3][2] - proj[1][2], proj[3][3] - proj[1][3]);
	nearPlane =		glm::vec4(proj[3][0] + proj[2][0], proj[3][1] + proj[2][1], proj[3][2] + proj[2][2], proj[3][3] + proj[2][3]);
	farPlane =		glm::vec4(proj[3][0] - proj[2][0], proj[3][1] - proj[2][1], proj[3][2] - proj[2][2], proj[3][3] - proj[2][3]);*/

	leftPlane.a =	proj[3][0] + proj[0][0];
	leftPlane.b =	proj[3][1] + proj[0][1];
	leftPlane.c =	proj[3][2] + proj[0][2];
	leftPlane.d =	proj[3][3] + proj[0][3];
	rightPlane.a =	proj[3][0] - proj[0][0];
	rightPlane.b =	proj[3][1] - proj[0][1];
	rightPlane.c =	proj[3][2] - proj[0][2];
	rightPlane.d =	proj[3][3] - proj[0][3];
	bottomPlane.a = proj[3][0] + proj[1][0];
	bottomPlane.b = proj[3][1] + proj[1][1];
	bottomPlane.c = proj[3][2] + proj[1][2];
	bottomPlane.d = proj[3][3] + proj[1][3];
	topPlane.a =	proj[3][0] - proj[1][0];
	topPlane.b =	proj[3][1] - proj[1][1];
	topPlane.c =	proj[3][2] - proj[1][2];
	topPlane.d =	proj[3][3] - proj[1][3];
	nearPlane.a =	proj[3][0] + proj[2][0];
	nearPlane.b =	proj[3][1] + proj[2][1];
	nearPlane.c =	proj[3][2] + proj[2][2];
	nearPlane.d =	proj[3][3] + proj[2][3];
	farPlane.a =	proj[3][0] - proj[2][0];
	farPlane.b =	proj[3][1] - proj[2][1];
	farPlane.c =	proj[3][2] - proj[2][2];
	farPlane.d =	proj[3][3] - proj[2][3];
}

bool Frustum::isVertexIn(glm::vec4 v, PlaneType plane)
{
	bool isIn = false;

	switch (plane)
	{
	case PlaneType::LEFT:
		isIn = (0 <= (leftPlane.a * v.x + leftPlane.b * v.y + leftPlane.c * v.z + leftPlane.d));
		break;
	case PlaneType::RIGHT:
		isIn = (0 <= (rightPlane.a * v.x + rightPlane.b * v.y + rightPlane.c * v.z + rightPlane.d));
		break;
	case PlaneType::BOTTOM:
		isIn = (0 <= (bottomPlane.a * v.x + bottomPlane.b * v.y + bottomPlane.c * v.z + bottomPlane.d));
		break;
	case PlaneType::TOP:
		isIn = (0 <= (topPlane.a * v.x + topPlane.b * v.y + topPlane.c * v.z + topPlane.d));
		break;
	case PlaneType::NEAR:
		isIn = (0 <= (nearPlane.a * v.x + nearPlane.b * v.y + nearPlane.c * v.z + nearPlane.d));
		break;
	case PlaneType::FAR:
		isIn = (0 <= (farPlane.a * v.x + farPlane.b * v.y + farPlane.c * v.z + farPlane.d));
		break;
	}

	return isIn;
}

void Frustum::containsBlock(Block* block)
{
	float size = block->getSize().x;
	float x = block->getPosition().x;
	float y = block->getPosition().y;
	float z = block->getPosition().z;
	float w = 1.0f;
	int totalIn = 0;
	glm::vec4 v[8];
	v[0] = glm::vec4(x, y, z, w);
	v[1] = glm::vec4(x, y + size, z, w);
	v[2] = glm::vec4(x + size, y + size, z, w);
	v[3] = glm::vec4(x + size, y, z, w);
	v[4] = glm::vec4(x, y, z + size, w);
	v[5] = glm::vec4(x, y + size, z + size, w);
	v[6] = glm::vec4(x + size, y + size, z + size, w);
	v[7] = glm::vec4(x + size, y, z + size, w);

	bool isLeft, isRight, isBottom, isTop, isNear, isFar;
	bool vertexActive[8];
	for (int i = 0; i < 8; i++)
	{
		bool isLeft =	isVertexIn(v[i], PlaneType::LEFT);
		bool isRight =	isVertexIn(v[i], PlaneType::RIGHT);
		bool isBottom = isVertexIn(v[i], PlaneType::BOTTOM);
		bool isTop =	isVertexIn(v[i], PlaneType::TOP);
		bool isNear =	isVertexIn(v[i], PlaneType::NEAR);
		bool isFar =	isVertexIn(v[i], PlaneType::FAR);
		
		if (isLeft && isRight && isBottom && isTop && isNear && isFar)	// if AT LEAST one vertex is in frustum, then setActive(true)
		{
			block->setActive(true);
			break;
		}
	}
	
	// transform vertex into perspective 

}