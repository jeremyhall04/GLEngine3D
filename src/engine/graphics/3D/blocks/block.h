#ifndef BLOCK_H
#define BLOCK_H

#include "../../../GLcommon.h"
#include "../renderable3D.h"

namespace delta { namespace graphics {

	enum BlockType {
		BlockType_Default = 0,
		BlockType_Grass
	};

	const GLfloat BLOCK_VERTICES[24] = {
		0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,

		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1
	};
	const GLuint BLOCK_INDICES[36] = {
		1, 0, 3, 1, 3, 2, // north (-z)
		4, 5, 6, 4, 6, 7, // south (+z)
		5, 1, 2, 5, 2, 6, // east (+x)
		0, 4, 7, 0, 7, 3, // west (-x)
		2, 3, 7, 2, 7, 6, // top (+y)
		5, 4, 0, 5, 0, 1, // bottom (-y)
	};

	class Block : public Renderable3D
	{
	public:
		Block();
		Block(float x, float y, float z, float cube_size, const glm::vec4 color);
	};

}}

#endif // !BLOCK_H

