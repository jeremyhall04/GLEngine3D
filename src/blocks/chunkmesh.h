#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include "../graphics/renderer3d.h"
#include "../graphics/buffers/VAO.h"
#include "../graphics/buffers/VBO.h"

class Chunk; // forward declaration

//struct ChunkMeshBuffer
//{
//	void* data;
//	GLuint capacity;// capacity (in bytes) of data
//	GLuint index;	// index (in bytes) into data (building mesh)
//	GLuint count;	// final count (in bytes) in data
//	GLuint currentCount;
//};

struct BlockMeshBuffer
{
	glm::vec3 pos;
	bool renderFaces[6] = { true };
};

class ChunkMesh
{
public:
	VAO vao;
	VBO vbo;
	Chunk* chunk;
	//ChunkMeshBuffer buffers;
	BlockMeshBuffer blockMesh;
	VertexData* meshData;
	size_t meshSize;
	GLuint index;


	ChunkMesh();
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();

	void generateMesh();

	void generateBlockMesh(int i, int j, int k);
	void updateMesh();
};

void renderChunkMesh(ChunkMesh* mesh);

#endif // !CHUNK_MESH_H

