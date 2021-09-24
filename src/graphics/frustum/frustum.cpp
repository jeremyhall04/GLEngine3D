#include "frustum.h"
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void Plane::setNormal()
{
	normal = glm::vec3(a, b, c);
}

const glm::vec3& Plane::getNormal(bool isNormalized)
{
	float mag = 1.0f;
	if (isNormalized)
		mag = sqrt(a * a + b * b + c * c);
	return normal / mag;
}

float Plane::getDistanceToPoint(const glm::vec4& v)
{
	return a * v.x + b * v.y + c * v.z + d;
}

Frustum::Frustum()
{
}

Frustum::Frustum(PerspectiveCamera* cam)
	: camera(cam)
{
	// http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
	updatePlanes();
}

Frustum::~Frustum()
{
}

void Frustum::updatePlanes()
{
	glm::mat4 proj = camera->getProjectionMatrix() * camera->getViewMatrix();
	glm::vec4 row1 = glm::row(proj, 0);
	glm::vec4 row2 = glm::row(proj, 1);
	glm::vec4 row3 = glm::row(proj, 2);
	glm::vec4 row4 = glm::row(proj, 3);

	// row3 + row0
	leftPlane.a =	proj[0][3] + proj[0][0];
	leftPlane.b =	proj[1][3] + proj[1][0];
	leftPlane.c =	proj[2][3] + proj[2][0];
	leftPlane.d =	proj[3][3] + proj[3][0];
	leftPlane.setNormal();

	// row3 - row0
	rightPlane.a =	proj[0][3] - proj[0][0];
	rightPlane.b =	proj[1][3] - proj[1][0];
	rightPlane.c =	proj[2][3] - proj[2][0];
	rightPlane.d =	proj[3][3] - proj[3][0];
	rightPlane.setNormal();

	// row3 + row1
	bottomPlane.a = proj[0][3] + proj[0][1];
	bottomPlane.b = proj[1][3] + proj[1][1];
	bottomPlane.c = proj[2][3] + proj[2][1];
	bottomPlane.d = proj[3][3] + proj[3][1];
	bottomPlane.setNormal();

	// row3 - row1
	topPlane.a =	proj[0][3] - proj[0][1];
	topPlane.b =	proj[1][3] - proj[1][1];
	topPlane.c =	proj[2][3] - proj[2][1];
	topPlane.d =	proj[3][3] - proj[3][1];
	topPlane.setNormal();

	// row3 + row2
	nearPlane.a =	proj[0][3] + proj[0][2];
	nearPlane.b =	proj[1][3] + proj[1][2];
	nearPlane.c =	proj[2][3] + proj[2][2];
	nearPlane.d =	proj[3][3] + proj[3][2];
	nearPlane.setNormal();

	// row3 - row2
	farPlane.a =	proj[0][3] - proj[0][2];
	farPlane.b =	proj[1][3] - proj[1][2];
	farPlane.c =	proj[2][3] - proj[2][2];
	farPlane.d =	proj[3][3] - proj[3][2];
	farPlane.setNormal();

	/*
	m_Planes[0] = leftPlane;
	m_Planes[1] = rightPlane;
	m_Planes[2] = bottomPlane;
	m_Planes[3] = topPlane;
	m_Planes[4] = nearPlane;
	m_Planes[5] = farPlane;

	for (int i = 0; i < 6; i++)
	{
		printf("\nplane[%d] = %f, %f, %f, %f", i, m_Planes[i].a, m_Planes[i].b, m_Planes[i].c, m_Planes[i].d);
	}

	// column0 + column3
	//leftPlane.a =	row4[0] + row1[0];
	//leftPlane.b =	row4[1] + row1[1];
	//leftPlane.c =	row4[2] + row1[2];
	//leftPlane.d =	row4[3] + row1[3];
	//rightPlane.a =	row4[0] - row1[0];
	//rightPlane.b =	row4[1] - row1[1];
	//rightPlane.c =	row4[2] - row1[2];
	//rightPlane.d =	row4[3] - row1[3];
	//
	//bottomPlane.a = row4[0] + row2[0];
	//bottomPlane.b = row4[1] + row2[1];
	//bottomPlane.c = row4[2] + row2[2];
	//bottomPlane.d = row4[3] + row2[3];
	//topPlane.a =	row4[0] - row2[0];
	//topPlane.b =	row4[1] - row2[1];
	//topPlane.c =	row4[2] - row2[2];
	//topPlane.d =	row4[3] - row2[3];

	//nearPlane.a =	row4[0] + row3[0];
	//nearPlane.b =	row4[1] + row3[1];
	//nearPlane.c =	row4[2] + row3[2];
	//nearPlane.d =	row4[3] + row3[3];
	//farPlane.a =	row4[0] - row3[0];
	//farPlane.b =	row4[1] - row3[1];
	//farPlane.c =	row4[2] - row3[2];
	//farPlane.d =	row4[3] - row3[3];*/

	m_Planes[0] = leftPlane;
	m_Planes[1] = rightPlane;
	m_Planes[2] = bottomPlane;
	m_Planes[3] = topPlane;
	m_Planes[4] = nearPlane;
	m_Planes[5] = farPlane;

}

bool Frustum::isVertexIn(glm::vec4 v, PlaneType plane)
{
	bool isIn = false;

	switch (plane)
	{
	case PlaneType::LEFT:
		isIn = (0 < (leftPlane.a * v.x + leftPlane.b * v.y + leftPlane.c * v.z + leftPlane.d));
		break;
	case PlaneType::RIGHT:
		isIn = (0 < (rightPlane.a * v.x + rightPlane.b * v.y + rightPlane.c * v.z + rightPlane.d));
		break;
	case PlaneType::BOTTOM:
		isIn = (0 < (bottomPlane.a * v.x + bottomPlane.b * v.y + bottomPlane.c * v.z + bottomPlane.d));
		break;
	case PlaneType::TOP:
		isIn = (0 < (topPlane.a * v.x + topPlane.b * v.y + topPlane.c * v.z + topPlane.d));
		break;
	case PlaneType::NEAR:
		isIn = (0 < (nearPlane.a * v.x + nearPlane.b * v.y + nearPlane.c * v.z + nearPlane.d));
		break;
	case PlaneType::FAR:
		isIn = (0 < (farPlane.a * v.x + farPlane.b * v.y + farPlane.c * v.z + farPlane.d));
		break;
	}
	printf(" = %d", isIn);
	return isIn;
}

bool Frustum::containsBlock(Block* block)
{
	updatePlanes();
	float size = block->getSize().x;
	float x = block->getPosition().x;
	float y = block->getPosition().y;
	float z = block->getPosition().z;
	float w = 1.0f;
	glm::vec4 v[8];
	v[0] = glm::vec4(x, y, z, w);
	v[1] = glm::vec4(x, y + size, z, w);
	v[2] = glm::vec4(x + size, y + size, z, w);
	v[3] = glm::vec4(x + size, y, z, w);
	v[4] = glm::vec4(x, y, z + size, w);
	v[5] = glm::vec4(x, y + size, z + size, w);
	v[6] = glm::vec4(x + size, y + size, z + size, w);
	v[7] = glm::vec4(x + size, y, z + size, w);

	bool isRendered = true;	// render unless completely behind a plane
	int withinPlanes = 0;

	// test 8 corners against 6 planes
	// if all points are behind 1 specific plane, is outside
	// if inside, fully in
	for (int p = 0; p < 6; p++)
	{
		int vertexIn = 8;
		int PtIn = 1;

		for (int i = 0; i < 8; i++)
		{
			if (m_Planes[p].getDistanceToPoint(v[i]) < 0)	// if point is negative halfspace
			{
 				PtIn = 0;
				vertexIn--;
			}
		}

		// were all the points outside plane p?
		if (vertexIn == 0)
		{
			//block->setActive(false);
			isRendered = false;
			break;
		}

		withinPlanes += PtIn;
	}

	if (withinPlanes == 6)
		isRendered = true;
		//block->setActive(true);
	
	return isRendered;
	//else intersect
}