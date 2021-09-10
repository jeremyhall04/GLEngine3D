#include "renderer3d.h"

namespace delta { namespace graphics {

	Renderer3D::Renderer3D()
	{
		m_TransformationStack.push_back(glm::mat4(1.0f));
		m_TransformationBack = &m_TransformationStack.back();
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
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE_3D, NULL, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX_3D);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX_3D);
		glVertexAttribPointer(SHADER_VERTEX_INDEX_3D, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE_3D, (const GLvoid*)0);
		glVertexAttribPointer(SHADER_COLOR_INDEX_3D, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE_3D, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::color)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLushort* indices = new GLushort[RENDERER_INDICES_SIZE_3D];
		/*for (int i = 0; i < RENDERER_INDICES_SIZE_3D; i += 36)
		{
			indices[  i  ] = 0;
			indices[i + 1] = 1;
			indices[i + 2] = 3;
			indices[i + 3] = 3;
			indices[i + 4] = 1;
			indices[i + 5] = 2;
		
			indices[i + 6] = 1;
			indices[i + 7] = 5;
			indices[i + 8] = 2;
			indices[i + 9] = 2;
			indices[i + 10] = 5;
			indices[i + 11] = 6;

			indices[i + 12] = 5;
			indices[i + 13] = 4;
			indices[i + 14] = 6;
			indices[i + 15] = 6;
			indices[i + 16] = 4;
			indices[i + 17] = 7;

			indices[i + 18] = 4;
			indices[i + 19] = 0;
			indices[i + 20] = 7;
			indices[i + 21] = 7;
			indices[i + 22] = 0;
			indices[i + 23] = 3;

			indices[i + 24] = 3;
			indices[i + 25] = 2;
			indices[i + 26] = 7;
			indices[i + 27] = 7;
			indices[i + 28] = 2;
			indices[i + 29] = 6;

			indices[i + 30] = 4;
			indices[i + 31] = 5;
			indices[i + 32] = 0;
			indices[i + 33] = 0;
			indices[i + 34] = 5;
			indices[i + 35] = 1;
		}*/

		for (int i = 0; i < RENDERER_INDICES_SIZE_3D; i += 36)
			for (int j = 0; j < 36; j++)
				indices[i + j] = CUBE_INDICES[j];

		m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE_3D);

		glBindVertexArray(0);
	}

	void Renderer3D::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (VertexData3D*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}

	void Renderer3D::submit(const Renderable3D* renderable)
	{
		const glm::vec4& position = renderable->getPosition();
		const glm::vec3& size = renderable->getSize();
		const glm::vec4& color = renderable->getColor();

		int r = color.x * 255;
		int g = color.y * 255;
		int b = color.z * 255;
		int a = color.w * 255;

		GLuint c = a << 24 | b << 16 | g << 8 | r;

		// COUNTER CLOCKWISE 

		/*// Front face
		m_Buffer->vertex = *m_TransformationBack * position;																	// bottom left																	
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x + size.x, position.y, position.z, position.w);			// bottom right
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x + size.x, position.y + size.y, position.z, position.w);	// top left
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x, position.y + size.y, position.z, position.w);			// top right
		m_Buffer->color = c;
		m_Buffer++;


		// Back face
		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x, position.y, position.z + size.z, position.w);						// bottom right
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x + size.x, position.y, position.z + size.z, position.w);				// bottom left
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x + size.x, position.y + size.y, position.z + size.z, position.w);	// top right
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * glm::vec4(position.x, position.y + size.y, position.z + size.z, position.w);				// top left
		m_Buffer->color = c;
		m_Buffer++;*/

		for (int i = 0; i < 8; i++)
		{
			const float* vertex = &CUBE_VERTICES[i * 3];
			m_Buffer->vertex = /**m_TransformationBack * */glm::vec4(position.x + vertex[0], position.y + vertex[1], position.z + vertex[1], position.w);
			m_Buffer->color;
			m_Buffer++;
		}

		m_IndexCount += 36;
	}

	void Renderer3D::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind
	}

	void Renderer3D::flush()
	{
		glBindVertexArray(m_VAO);
		m_IBO->bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, NULL);

		m_IBO->unbind();
		glBindVertexArray(0);

		m_IndexCount = 0; // reset count
	}

}}