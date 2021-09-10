#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <cstddef>
#include "../renderer.h"
#include "block/block.h"

#define RENDERER_MAX_SPRITES_3D		1000
#define RENDERER_VERTEX_SIZE_3D		sizeof(VertexData3D)
#define RENDERER_SPRITE_SIZE_3D		RENDERER_VERTEX_SIZE_3D * 8
#define RENDERER_BUFFER_SIZE_3D		RENDERER_SPRITE_SIZE_3D * RENDERER_MAX_SPRITES_3D
#define RENDERER_INDICES_SIZE_3D	RENDERER_MAX_SPRITES_3D * 36

#define SHADER_VERTEX_INDEX_3D 0
#define SHADER_COLOR_INDEX_3D 1

namespace delta { namespace graphics {

	class Renderer3D : public Renderer
	{
	protected:
		std::vector<glm::mat4> m_TransformationStack;
		const glm::mat4* m_TransformationBack;
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		IndexBuffer* m_IBO;
		GLsizei m_IndexCount;
		VertexData3D* m_Buffer;

	public:
		Renderer3D();
		~Renderer3D();

	public:
		inline void push(glm::mat4 matrix, bool override = false)
		{
			if (override)
				m_TransformationStack.push_back(matrix);
			else
				m_TransformationStack.push_back(m_TransformationStack.back() * matrix);

			m_TransformationBack = &m_TransformationStack.back();
		}
		inline void pop()
		{
			// TODO: add to log!
			if (m_TransformationStack.size() > 1)
				m_TransformationStack.pop_back();

			m_TransformationBack = &m_TransformationStack.back();
		}

	private:
		void init();
	public:
		void begin() override;
		void submit(const Renderable3D* renderable);
		void end() override;
		void flush() override;
	};

}}
#endif // !RENDERER_2D_H

