#include "chunk.h"

namespace delta { namespace graphics {

	Chunk::Chunk(Shader* shader)
		: Layer3D(new Renderer3D(), shader, glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 100.0f))
	{
	}

	Chunk::Chunk(Shader* shader, bool isOrthogonal)
		: Layer3D(new Renderer3D(), shader, glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
	{
	}

	Chunk::~Chunk()
	{
	}

}}