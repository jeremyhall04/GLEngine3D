#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &handle);
	type = GL_ARRAY_BUFFER;
}

VBO::VBO(GLint bufferType)
	: type(bufferType)
{
	glGenBuffers(1, &handle);
}

VBO::~VBO()
{
	glDeleteBuffers(1, &handle);
}

void VBO::bind()
{
	glBindBuffer(type, handle);
}

void VBO::unbind()
{
	glBindBuffer(type, 0);
}