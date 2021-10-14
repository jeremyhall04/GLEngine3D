#ifndef WORLD_H
#define WORLD_H

#include "../blocks/chunk.h"
#include <vector>

#define WORLD_WIDTH		3
#define WORLD_HEIGHT	2
#define WORLD_HEIGHT_P (int)glm::floor(WORLD_HEIGHT / 2.0)
#define WORLD_HEIGHT_N (int)(-1 * glm::ceil(WORLD_HEIGHT / 2.0))
#define WORLD_DEPTH		3
#define WORLD_VOLUME (WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH)

class Player;

class World
{
private:
	bool blockRemove = false, blockPlace = false;
	std::vector<Chunk*> chunkUpdateList;

public:
	Player* player;

public:
	Chunk**** chunks;
	World();
	World(Player* player);
	~World();

	void initialize();
	void update();
	void mouseEvent(int button, int action);

private:
	void updateWorldFirstPass();
};

const glm::vec3 pos_to_world_chunk_coord(float x, float y, float z);
const glm::vec3 pos_to_world_chunk_coord(const glm::vec3& pos);
void pos_to_world_chunk(World* world, const glm::vec3& pos, Chunk* c);

#endif // !WORLD_H
