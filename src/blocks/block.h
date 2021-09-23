#ifndef BLOCK_H
#define BLOCK_H

#include "../GLcommon.h"
#include "../graphics/renderable3D.h"

struct BlockFaceDir {

};

class Block : public Renderable3D
{
private:
	bool m_isActive;
	BlockType m_BlockType;

public:
	int m_ID;
	
	Block();
	Block(float x, float y, float z, float cube_size, const glm::vec4 color);
	Block(float x, float y, float z, float cube_size, BlockType blockType);
	
	bool isActive() { return m_isActive; };
	bool const isActive() const { return m_isActive; };
	void setActive(bool active) { m_isActive = active; };
};

void add_to_blocks(const Block& block);
void remove_from_blocks(GLuint blockIndex);

#endif // !BLOCK_H

