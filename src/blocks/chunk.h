#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
//#include "chunkmesh.h"

#define CHUNK_SIZE 4
#define CHUNK_SIZE_SQUARED (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_SIZE_CUBED (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

#define CHUNK_VERTEX_SIZE	sizeof(VertexData3D)
#define CHUNK_BLOCK_SIZE	CHUNK_VERTEX_SIZE * 108
#define CHUNK_BUFFER_SIZE	CHUNK_BLOCK_SIZE * CHUNK_SIZE_CUBED

#define INDEX_INBOUNDS(x) (x < CHUNK_SIZE - 1 ? x : CHUNK_SIZE - 1)

//#define chunk_position_to_index(p) (p.x * CHUNK_SIZE.x * CHUNK_SIZE.z + p.z * CHUNK_SIZE.z + p.y)

class World;

struct ChunkHelper
{
	bool visitedXN[CHUNK_SIZE_CUBED];
	bool visitedXP[CHUNK_SIZE_CUBED];
	bool visitedYN[CHUNK_SIZE_CUBED];
	bool visitedYP[CHUNK_SIZE_CUBED];
	bool visitedZN[CHUNK_SIZE_CUBED];
	bool visitedZP[CHUNK_SIZE_CUBED];
};

class Chunk
{
public:
	//glm::vec3 offset, position;	// offset in world chunks, position of plock
	//GLuint blockCount;
	Block** data;
	int chunkX, chunkY, chunkZ;
	bool isEmpty = false;
	bool isModified = false;
	bool isGenerating = false;

private:
	World* world;
	//Chunk * cXN, * cXP, * cYN, * cYP, * cZN, * cZP;

public:
	Chunk();
	Chunk(World* world, int i, int j, int k);
	~Chunk();

	void generateChunkData();
	Block* get_block_from_pos_in_chunk(int i, int j, int k);
private:
	const glm::vec3& to_global_coord(int i, int j, int k);
/*private:
	void generateMesh(ChunkHelper helper);
	void createRuns(Block* block, int i, int j, int k, ChunkHelper& helper, int access);
	bool visibleFaceXN(int i, int j, int k);
	bool visibleFaceXP(int i, int j, int k);
	bool isDifferentBlock(int access, Block& current);*/
};

int to_data_index(int i, int j, int k);
void updateChunkBlockFaces(Chunk* chunk);

#endif // !CHUNK_H

