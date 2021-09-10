#ifndef BLOCK_H
#define BLOCK_H

#include "../../../GLcommon.h"
#include "../renderable3d.h"

namespace delta { namespace graphics {

	class Block : public Renderable3D
	{
	public:
		Block(float x, float y, float z, float width, float height, float depth, const glm::vec4& color);
		Block(float x, float y, float z, float cube_width, const glm::vec4& color);

	};

	static const int FACE_INDICES[6] = { 1, 0, 3, 1, 3, 2 };
	static const GLushort CUBE_INDICES[36] = {
		1, 0, 3, 1, 3, 2, // north (-z)
		4, 5, 6, 4, 6, 7, // south (+z)
		5, 1, 2, 5, 2, 6, // east (+x)
		0, 4, 7, 0, 7, 3, // west (-x)
		2, 3, 7, 2, 7, 6, // top (+y)
		5, 4, 0, 5, 0, 1, // bottom (-y)
	};
	
	static const float CUBE_VERTICES[] = {
		0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,

		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1
	};

}}

#endif // !BLOCK_H
