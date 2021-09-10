#ifndef CHUNK_H
#define CHUNK_H

#include "blocks/block.h"
#include "renderer3d.h"

namespace delta { namespace graphics {

	class Chunk
	{
	public:
		Chunk();
		~Chunk();
		void update();
		void render(Renderer3D* renderer);
		void createMesh();


		static const int CHUNK_SIZE = 16;
	private:
		Block*** m_Blocks;
	};

}}

#endif // !CHUNK_H
