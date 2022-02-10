#ifndef CHUNK_H
#define CHUNK_H

#include "block.h"
#include "chunkmesh.h"

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


// Redo chunks - i need seperate chunkdata & chunkmesh, so that only the chunkmesh is passed to the renderer
// i guess i would be accessing this chunk.data whenever the chunk is modified, but the chunk data instead of being block renderables
// the data is simply the block type (air, grass, dirt, ect..).
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
	ChunkMesh* mesh;

private:
	World* world;

public:
	Chunk();
	Chunk(int i, int j, int k);
	~Chunk();

	void generateChunkData();
	void removeChunkFaceFromRender(FaceDirection face);
	Block* getBlockFromIndex(int i, int j, int k);

private:
	const glm::vec3 to_global_coord(int i, int j, int k);
};

void index_inbounds(int& x);
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

