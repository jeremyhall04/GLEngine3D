#ifndef WORLD_H
#define WORLD_H

#include "../blocks/chunk.h"
#include <vector>

// world size in chunks
#define WORLD_WIDTH		2
#define WORLD_HEIGHT	3
#define WORLD_DEPTH		2
#define WORLD_VOLUME (WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH)

class Player;

struct WorldMeshBuffer
{
	VAO vao;
	VBO vbo;
	VertexData* worldMeshData;
	int index;
	size_t chunkMeshSize, worldSize;
	void initialize(size_t chunkMeshSize, size_t worldSize);
	void bind();
	void unbind();
	void submitChunkMesh(ChunkMesh* chunkMesh);
	void resubmitChunkMesh(ChunkMesh* chunkMesh, int index);
	void render();
};

class World
{
private:
	bool blockRemove = false, blockPlace = false;
	std::vector<Chunk*> chunkUpdateList;

public:
	Player* player;
	WorldMeshBuffer meshBuffer;
public:
	Chunk**** chunks;
	World();
	World(Player* player);
	~World();

	void initialize();
	void update(Player* player);
	void mouseEvent(int button, int action);

private:
	void updateWorldFirstPass();
};

const glm::vec3 pos_to_world_chunk_coord(float x, float y, float z);
const glm::vec3 pos_to_world_chunk_coord(const glm::vec3& pos);
void pos_to_world_chunk(World* world, const glm::vec3& pos, Chunk* c);

#endif // !WORLD_H
