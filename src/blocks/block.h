#ifndef BLOCK_H
#define BLOCK_H

#include "../GLcommon.h"
#include "../graphics/renderable3D.h"

struct BlockFaceDir {

};

class Block : public Renderable3D
{
public:
	int m_ID;
	bool isActive();
	void activate(bool active);

private:
	bool m_isActive;
	BlockType m_BlockType;

public:
	Block();
	Block(float x, float y, float z, float cube_size, const glm::vec4 color);
	Block(float x, float y, float z, float cube_size, BlockType blockType);
};

struct Blocks {
	Block** blocks;
	GLuint blockCount = 0;
};

extern Blocks WorldBlocks;

void add_to_blocks(const Block& block);
void remove_from_blocks(GLuint blockIndex);

#endif // !BLOCK_H

