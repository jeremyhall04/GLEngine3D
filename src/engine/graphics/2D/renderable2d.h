#ifndef RENDERABLE_2D_H
#define RENDERABLE_2D_H

#include "../../GLcommon.h"
#include "../../GLcommon_math.h"
#include "../buffers/buffer.h"
#include "../buffers/indexbuffer.h"
#include "../buffers/vertexarray.h"
#include "../shader.h"

namespace delta {namespace graphics {

	struct VertexData
	{
		glm::vec3 vertex;
		GLuint color;
	};

	class Renderable2D
	{
	public:
		glm::vec3 m_Position;
		glm::vec2 m_Size;
		glm::vec4 m_Color;

		Renderable2D(glm::vec3 position, glm::vec2 size, glm::vec4 color)
			: m_Position(position), m_Size(size), m_Color(color)
		{ };
		virtual ~Renderable2D() { }

		inline const glm::vec3& getPosition() const { return m_Position; };
		inline const glm::vec2& getSize() const { return m_Size; };
		inline const glm::vec4& getColor() const { return m_Color; };
	};

}}
#endif // !RENDERABLE_2D_H
