#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <cstddef>
#include "../renderer.h"
#include "renderable3D.h"
#include "blocks/block.h"

#define REN3D_MAX_BLOCKS	100
#define REN3D_VERTEX_SIZE	sizeof(VertexData3D)
#define REN3D_BLOCK_SIZE	REN3D_VERTEX_SIZE * 8
#define REN3D_BUFFER_SIZE	REN3D_BLOCK_SIZE * REN3D_MAX_BLOCKS
#define REN3D_INDICES_SIZE	REN3D_MAX_BLOCKS * 36

#define SHADER_VERTEX_INDEX 0
#define SHADER_COLOR_INDEX 1

namespace delta { namespace graphics {

	class Renderer3D : public Renderer
	{
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		IndexBuffer* m_IBO;
		GLsizei m_IndexCount;
		VertexData3D* m_VertexBuffer;

	public:
		Renderer3D();
		~Renderer3D();

	private:
		void init();
	public:
		void begin() override;
		void submit(const Renderable3D* renderable);
		void end() override;
		void flush() override;
	};

}}

#endif // !RENDERER_3D_H

