#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &handle);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &handle);
}

void VAO::bind()
{
	glBindVertexArray(handle);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::genBufferData(VBO* vbo, size_t dataSize, size_t dataStride, void* data)
{
	bind();
	vbo->bind();
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

	setAttrib(0, 3, GL_FLOAT, dataStride, 0);					// position (3 floats)
	setAttrib(1, 2, GL_FLOAT, dataStride, 3 * sizeof(float));	// uv		(2 floats)
	setAttrib(2, 1, GL_FLOAT, dataStride, 5 * sizeof(float));	// tid		(1 float)
}

void VAO::genBufferData(VBO* vbo, size_t dataSize, size_t dataStride)
{
	bind();
	vbo->bind();
	glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_DYNAMIC_DRAW);
	setAttrib(0, 3, GL_FLOAT, dataStride, 0);					// position (3 floats)
	setAttrib(1, 2, GL_FLOAT, dataStride, 3 * sizeof(float));	// uv		(2 floats)
	setAttrib(2, 1, GL_FLOAT, dataStride, 5 * sizeof(float));	// tid		(1 float)
}

void VAO::setAttrib(GLuint attributeIndex, size_t attributeSize, GLenum attributeType, size_t dataStride, size_t attributeOffset)
{
	glEnableVertexAttribArray(attributeIndex);
	glVertexAttribPointer(attributeIndex, attributeSize, attributeType, GL_FALSE, dataStride, (const GLvoid*)attributeOffset);
}
