#ifndef BLOCK_H
#define BLOCK_H

#include "../GLcommon.h"
#include "../graphics/renderable3D.h"

enum class BlockType {
	_Default	= -1,
	Grass		= 0,
	Sky			= 1
};

class Block : public Renderable3D
{
private:
	int m_ID;
public:
	Block();
	Block(float x, float y, float z, float cube_size, const glm::vec4 color);
	Block(float x, float y, float z, float cube_size, BlockType blockType);
};

#endif // !BLOCK_H

