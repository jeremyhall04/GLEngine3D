#ifndef BUFFER_H
#define BUFFER_H

#include "../../GLcommon.h"

class Buffer
{
private:
	GLuint m_BufferID;
	GLuint m_ComponentCount;
public:
	Buffer(GLfloat* data, GLsizei count, GLuint componentCount);
	~Buffer();

	void bind() const;
	void unbind() const;

	inline GLuint getComponentCount() const { return m_ComponentCount; };
};

#endif // !BUFFER_H


