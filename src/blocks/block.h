#ifndef BLOCK_H
#define BLOCK_H

#include "../GLcommon.h"
#include "../graphics/renderable3D.h"

enum class Face
{
	NORTH,
	SOUTH,
	EAST,
	WEST,
	UP,
	DOWN
};

class Block : public Renderable3D
{
public:	
	Block();
	Block(float x, float y, float z, float cube_size, const glm::vec4 color);
	Block(float x, float y, float z, float cube_size, BlockType blockType);

	void setFacesToRender(bool NX, bool PX, bool NY, bool PY, bool NZ, bool PZ) /*override*/;
};

#endif // !BLOCK_H

