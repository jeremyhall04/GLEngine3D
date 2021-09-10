#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "../../GLcommon.h"
#include "buffer.h"
#include <vector>

namespace delta { namespace graphics {
	
	class VertexArray
	{
	private: 
		GLuint m_ArrayID;
		std::vector<Buffer*> m_Buffers;

	public:
		VertexArray();
		~VertexArray();

		void addBuffer(Buffer* buffer, GLuint index);
		void bind() const;
		void unbind() const;
	};

} }

#endif // !VERTEX_ARRAY_H

