#ifndef CHUNK_H
#define CHUNK_H

#include "layer3d.h"
#include "../renderer3d.h"

namespace delta { namespace graphics {

	class Chunk : public Layer3D
	{
	public:
		Chunk(Shader* shader);
		Chunk(Shader* shader, bool isOrthogonal);
		~Chunk();
	};

}}

#endif // !CHUNK_H
