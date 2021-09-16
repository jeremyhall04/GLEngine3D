#ifndef BLOCK_H
#define BLOCK_H

#include "../GLcommon.h"
#include "../graphics/renderable3D.h"

enum BlockType {
	BlockType_Default = 0,
	BlockType_Grass
};

class Block : public Renderable3D
{
public:
	Block();
	Block(float x, float y, float z, float cube_size, const glm::vec4 color);
};

#endif // !BLOCK_H

