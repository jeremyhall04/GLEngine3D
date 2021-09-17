#include "block.h"

Block::Block()
{
}

Block::Block(float x, float y, float z, float cube_size, const glm::vec4 color)
	: Renderable3D(glm::vec4(x, y, z, 1.0f), glm::vec3(cube_size, cube_size, cube_size), color), m_ID((int)BlockType::_Default)
{
}
 
Block::Block(float x, float y, float z, float cube_size, BlockType blockType)
	: Renderable3D(glm::vec4(x, y, z, 1.0f), glm::vec3(cube_size, cube_size, cube_size), glm::vec4(1, 0, 1, 1)), m_ID((int)blockType)
{

}

