#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
//#include "chunkmesh.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZE_SQUARED (CHUNK_SIZE * CHUNK_SIZE)
#define CHUNK_SIZE_CUBED (CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE)

#define CHUNK_VERTEX_SIZE	sizeof(VertexData3D)
#define CHUNK_BLOCK_SIZE	CHUNK_VERTEX_SIZE * 108
#define CHUNK_BUFFER_SIZE	CHUNK_BLOCK_SIZE * CHUNK_SIZE_CUBED

#define INDEX_INBOUNDS(x) (x < CHUNK_SIZE - 1 ? x : CHUNK_SIZE - 1)
#define CHUNK_TO_WORLD_INDEX(x) (x = x / CHUNK_SIZE)

//#define chunk_position_to_index(p) (p.x * CHUNK_SIZE.x * CHUNK_SIZE.z + p.z * CHUNK_SIZE.z + p.y)

class World;

class Chunk
{
public:
	//glm::vec3 offset, position;	// offset in world chunks, position of plock
	//GLuint blockCount;
	Block** data;
	int chunkX, chunkY, chunkZ;
	bool isEmpty = false;
	bool isLoaded = true;
	bool isModified = false;
	bool isGenerating = false;
	Chunk * cXN, * cXP, * cYN, * cYP, * cZN, * cZP;

private:
	World* world;

public:
	Chunk();
	Chunk(int i, int j, int k);
	~Chunk();

	void generateChunkData();
	void removeChunkFaceFromRender(FaceDirection face);
	Block* getBlockFromIndex(int i, int j, int k);
	Chunk* getNeighbourChunk(FaceDirection direction);

private:
	const glm::vec3 to_global_coord(int i, int j, int k);
/*private:
	void generateMesh(ChunkHelper helper);
	void createRuns(Block* block, int i, int j, int k, ChunkHelper& helper, int access);
	bool visibleFaceXN(int i, int j, int k);
	bool visibleFaceXP(int i, int j, int k);
	bool isDifferentBlock(int access, Block& current);*/
};

const glm::vec3 world_pos_to_chunk_block_coord(Chunk* c, const glm::vec3& pos);
Block* world_pos_to_chunk_block(Chunk* c, const glm::vec3& pos);
int to_data_index(int i, int j, int k);
Block* get_block_from_index(Chunk* chunk, int i, int j, int k);
void updateChunkBlockFaces(Chunk* chunk);
void compareChunkInterface(Chunk* chunk, Chunk* neighbour, FaceDirection face);

void updateChunkFaces(Chunk* c);
void updateChunkFaces(Chunk* c, int blockAccess);
void addBlockToChunkRender(Chunk* c, glm::vec3 pos, Chunk* updateList, int listCount);

#endif // !CHUNK_H

