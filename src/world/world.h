#ifndef WORLD_H
#define WORLD_H

#include "../blocks/chunk.h"

#define WORLD_WIDTH 10
#define WORLD_HEIGHT 1
#define WORLD_DEPTH 10
#define WORLD_VOLUME (WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH)

class World
{
private:
public:
	Chunk**** chunks;
	World();
	~World();

	void update();
};

void updateChunkFacesToRender(World* world);
void updateChunkOnModify(World* world, Chunk* chunk);

#endif // !WORLD_H
