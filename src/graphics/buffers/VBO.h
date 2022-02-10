#ifndef VBO_H
#define VBO_H

#include "../../GLcommon.h"

class VBO
{
public:
	GLuint handle;
	GLint type;

	VBO();
	VBO(GLint bufferType);
	~VBO();

	void bind();
	void unbind();
};

#endif // !VBO_H
