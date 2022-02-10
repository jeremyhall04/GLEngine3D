#ifndef VAO_H
#define VAO_H

#include "../../GLcommon.h"
#include "VBO.h"

class VAO
{
public:
	GLuint handle;

	VAO();
	~VAO();

	void bind();
	void unbind();
	void genBufferData(VBO* vbo, size_t dataSize, size_t dataStride, void* data);
	void genBufferData(VBO* vbo, size_t dataSize, size_t dataStride);
	void setAttrib(GLuint attributeIndex, size_t size, GLenum type, size_t stride, size_t offset);
};

#endif // !VAO_H
