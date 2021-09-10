#include "block.h"

namespace delta { namespace graphics {

	Block::Block(float x, float y, float z, float cube_size, const glm::vec4 color)
		: Renderable3D(glm::vec4(x, y, z, 1.0f), glm::vec3(cube_size, cube_size, cube_size), color)
	{
		
	}

}}

