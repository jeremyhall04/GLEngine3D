#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include "../../GLcommon.h"

namespace delta { namespace graphics {
	
	class IndexBuffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_Count;

	public:
		IndexBuffer(GLuint* data, GLuint count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline GLuint getCount() const { return m_Count; };
	};

} }
#endif // !INDEX_BUFFER_H

