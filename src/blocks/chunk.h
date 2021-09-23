#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
//#include "chunkmesh.h"

#include "../graphics/renderer3d.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16
#define CHUNK_SIZE_MAX 16
#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

#define CHUNK_VERTEX_SIZE	sizeof(VertexData3D)
#define CHUNK_BLOCK_SIZE	CHUNK_VERTEX_SIZE * 108
#define CHUNK_BUFFER_SIZE	CHUNK_BLOCK_SIZE * CHUNK_VOLUME

#define CHUNK_VERTEX_INDEX	0
#define CHUNK_NORMAL_INDEX	1
#define CHUNK_UV_INDEX		2
#define CHUNK_TID_INDEX		3
#define CHUNK_COLOR_INDEX	4

//#define chunk_position_to_index(p) (p.x * CHUNK_SIZE.x * CHUNK_SIZE.z + p.z * CHUNK_SIZE.z + p.y)

class Chunk
{
private:
	GLuint m_VAO;
	GLuint m_VBO;
	VertexData3D* m_VertexBuffer;

public:
	glm::vec3 offset, position;	// offset in world chunks, position of plock
	GLuint blockCount;
	Block*** chunkBlocks;
	bool isEmpty = false;
	bool isGenerated = false;
	bool isRender = true;

public:
	Chunk(float offsetX, float offsetY, float offsetZ);
	~Chunk();

	void init();
	void generateChunk(BlockType type);
private:
	void initVAO();
};

void occlusionCull(Chunk* chunk, glm::vec3 position);
void frustumCull(Chunk* chunk);

Block* get_block_from_position(const Chunk* chunk, const glm::vec3& position);
void generate_chunk(Chunk* chunk);
void chunk_on_modify();

#endif // !CHUNK_H

