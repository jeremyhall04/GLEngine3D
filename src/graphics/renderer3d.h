#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <cstddef>
#include "../GLcommon.h"
#include "../GLcommon_math.h"
#include "renderable3D.h"
#include "../blocks/block.h"
#include "texture/texture.h"

#define REN3D_MAX_BLOCKS		50000 
#define REN3D_VERTEX_SIZE		sizeof(VertexData3D)				// 20 bytes
#define REN3D_BLOCK_SIZE		REN3D_VERTEX_SIZE * 108
#define REN3D_BUFFER_SIZE		REN3D_BLOCK_SIZE * REN3D_MAX_BLOCKS
#define REN3D_MAX_INDICES		REN3D_MAX_BLOCKS * 108

#define SHADER_VERTEX_INDEX_3D 0
#define SHADER_UV_INDEX_3D		1
#define SHADER_COLOR_INDEX_3D	2

class Renderer3D
{
private:
	GLuint m_VAO;
	GLuint m_VBO;
	IndexBuffer* m_IBO;
	GLsizei m_IndexCount;
	VertexData3D* m_VertexBuffer;
	GLuint* activeBlocks;

public:
	Renderer3D();
	~Renderer3D();

private:
	void init();
	GLuint* allocateBlockIndices();
	GLuint* allocate108BlockIndices();
	const glm::vec3* checkFaceToRender();

public:
	VertexData3D* getVertexBuffer() { return m_VertexBuffer; };
	void begin();
	void submit(const Renderable3D* renderable);
	void end();
	void flush();
};


#endif // !RENDERER_3D_H

