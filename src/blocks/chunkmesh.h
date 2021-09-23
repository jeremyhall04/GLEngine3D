#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include "../graphics/renderer3d.h"

class Chunk; // forward declaration

struct ChunkMeshBuffer
{
	void* data;
	GLuint capacity;// capacity (in bytes) of data
	GLuint index;	// index (in bytes) into data (building mesh)
	GLuint count;	// final count (in bytes) in data
	GLuint currentCount;
};

class ChunkMesh
{
	Chunk* chunk;
	ChunkMeshBuffer buffers;

	Renderer3D renderer;
};

#endif // !CHUNK_MESH_H

