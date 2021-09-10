#include "renderer3d.h"

namespace delta { namespace graphics {

	Renderer3D::Renderer3D()
	{
		init();
	}

	Renderer3D::~Renderer3D()
	{
		delete m_IBO;
		glDeleteBuffers(1, &m_VBO);
	}

	void Renderer3D::init()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, REN3D_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);
		glVertexAttribPointer(SHADER_VERTEX_INDEX, 4, GL_FLOAT, GL_FALSE, REN3D_VERTEX_SIZE, (const GLvoid*)0);
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, REN3D_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::color)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);	// unbind array buffer

		GLushort* indices = new GLushort[REN3D_INDICES_SIZE];
		for (int i = 0; i < REN3D_INDICES_SIZE; i += 36)
			for (int j = 0; j < 36; j++)
				indices[i + j] = BLOCK_INDICES[j];

		m_IBO = new IndexBuffer(indices, REN3D_INDICES_SIZE);

		glBindVertexArray(0);
	}

	void Renderer3D::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_VertexBuffer = (VertexData3D*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void Renderer3D::submit(const Renderable3D* renderable)
	{
		const glm::vec4 position = renderable->getPosition();
		const glm::vec3 size = renderable->getSize();
		const glm::vec4 color = renderable->getColor();

		float width = size.x;
		float height = size.y;
		float depth = size.z;

		int r = color.x * 255;
		int g = color.y * 255;
		int b = color.z * 255;
		int a = color.w * 255;

		GLuint c = a << 24 | b << 16 | g << 8 | r;

		for (GLuint i = 0; i < 8; i++)
		{
			const float* vertex = &BLOCK_VERTICES[i * 3];
			m_VertexBuffer->vertex = /**m_TransformationBack * */glm::vec4(position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth, position.w);
			m_VertexBuffer->color = c;
			m_VertexBuffer++;
		}

		m_IndexCount += 36;
	}

	void Renderer3D::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
	}

	void Renderer3D::flush()
	{
		glBindVertexArray(m_VAO);
		m_IBO->bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, NULL);

		m_IBO->unbind();
		glBindVertexArray(0);

		m_IndexCount = 0;
	}

}}