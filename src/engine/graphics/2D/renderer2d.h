#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "../renderer.h"
#include "renderable2d.h"

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_VERTEX_SIZE	sizeof(VertexData)
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_COLOR_INDEX 1

namespace delta { namespace graphics {

	class Renderer2D : public Renderer
	{
	private:
		GLuint m_VAO;
		GLuint m_VBO;
		IndexBuffer* m_IBO;
		GLsizei m_IndexCount;
		VertexData* m_Buffer;

	public:
		Renderer2D();
		~Renderer2D();

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
		void submit(const Renderable2D* renderable);
		void end() override;
		void flush() override;
	};

}}
#endif // !RENDERER_2D_H

