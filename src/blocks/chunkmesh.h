#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

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
	ChunkMeshBuffer buffers[];

	
};

#endif // !CHUNK_MESH_H

