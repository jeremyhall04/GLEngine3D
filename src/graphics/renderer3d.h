#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <cstddef>
#include <vector>
#include "../GLcommon.h"
#include "../GLcommon_math.h"
#include "renderable3D.h"
#include "../blocks/block.h"
#include "texture/texture.h"
#include "shader.h"

#define MAX_BLOCKS		50000 
#define VERTEX_SIZE		sizeof(VertexData3D)				// 20 bytes
#define BLOCK_SIZE		VERTEX_SIZE * 108
#define BUFFER_SIZE		BLOCK_SIZE * MAX_BLOCKS
#define MAX_INDICES		MAX_BLOCKS * 108
#define MAX_TEXTURES	3

#define SHADER_VERTEX_INDEX_3D	0
#define SHADER_UV_INDEX_3D		1
#define SHADER_TID_INDEX_3D		2
#define SHADER_COLOR_INDEX_3D	3

class Renderer3D
{
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLsizei m_IndexCount;
	IndexBuffer* m_IBO;
	VertexData3D* m_VertexBuffer;

	GLuint* m_TextureSlotsPtr;
	GLuint m_TextureSlotIndex = 0;
	std::vector<GLuint> m_TextureSlotsVec;

public:
	Shader* shader;

public:
	Renderer3D();
	~Renderer3D();

private:
	void init();
	void generateTextureIndices();
	GLuint* allocateBlockIndices();
	GLuint* allocate108BlockIndices();

public:
	VertexData3D* getVertexBuffer() { return m_VertexBuffer; };
	void begin();
	void submit(const Renderable3D* renderable);
	void end();
	void flush();
};


#endif // !RENDERER_3D_H

