#include "block.h"

Block::Block()
{
}

Block::Block(float x, float y, float z, float cube_size, const glm::vec4 color)
	: Renderable3D(glm::vec3(x, y, z), glm::vec3(cube_size, cube_size, cube_size), color, BlockType::Air)
{
}
 
Block::Block(float x, float y, float z, float cube_size, BlockType blockType)
	: Renderable3D(glm::vec3(x, y, z), glm::vec3(cube_size, cube_size, cube_size), glm::vec4(1, 0, 1, 1), blockType)
{
}

void Block::setFacesToRender(bool NX, bool PX, bool NY, bool PY, bool NZ, bool PZ)
{
	renderFace[0] = NZ;
	renderFace[1] = PZ;
	renderFace[2] = PX;
	renderFace[3] = NX;
	renderFace[4] = PY;
	renderFace[5] = NY;
}
