#ifndef RENDERABLE_3D_H
#define RENDERABLE_3D_H

#include "../../GLcommon.h"
#include "../../GLcommon_math.h"
#include "../buffers/buffer.h"
#include "../buffers/indexbuffer.h"
#include "../buffers/vertexarray.h"
#include "../shader.h"

namespace delta {namespace graphics {

	struct VertexData3D
	{
		glm::vec4 vertex;
		GLuint color;
	};

	class Renderable3D
	{
	public:
		glm::vec4 m_Position;
		glm::vec3 m_Size;
		glm::vec4 m_Color;
//		Renderable3D() {};
		Renderable3D(glm::vec4 position, glm::vec3 size, glm::vec4 color)
			: m_Position(position), m_Size(size), m_Color(color)
		{ };
		virtual ~Renderable3D() { }

		inline const glm::vec4& getPosition() const { return m_Position; };
		inline const glm::vec3& getSize() const { return m_Size; };
		inline const glm::vec4& getColor() const { return m_Color; };
	};

}}
#endif // !RENDERABLE_3D_H
