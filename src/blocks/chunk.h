#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "chunkmesh.h"

#define CHUNK_SIZE_X 16
#define CHUNK_SIZE_Y 16
#define CHUNK_SIZE_Z 16
#define CHUNK_SIZE_MAX 16
#define CHUNK_VOLUME (CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z)

#define chunk_position_to_index(p) (p.x * CHUNK_SIZE.x * CHUNK_SIZE.z + p.z * CHUNK_SIZE.z + p.y)

class Chunk
{
public:
	glm::vec3 offset, position;	// offset in world chunks, position of plock
	GLuint count;

	bool empty : 1;
	ChunkMesh* mesh;
};

#endif // !CHUNK_H

