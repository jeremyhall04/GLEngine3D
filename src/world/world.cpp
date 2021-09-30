#include "world.h"

World::World()
{
	chunks = new Chunk * * * [WORLD_WIDTH];
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		chunks[i] = new Chunk * * [WORLD_HEIGHT];
		for (int j = 0; j < WORLD_HEIGHT; j++)
			chunks[i][j] = new Chunk * [WORLD_DEPTH];
	}

	for (int i = 0; i < WORLD_WIDTH; i++)
		for (int j = 0; j < WORLD_HEIGHT; j++)
			for (int k = 0; k < WORLD_DEPTH; k++)
				chunks[i][j][k] = new Chunk(i, j - WORLD_HEIGHT, k);

	// assign references to neighbouring chunks
	for (int i = 0; i < WORLD_WIDTH; i++)
		for (int j = 0; j < WORLD_HEIGHT; j++)
			for (int k = 0; k < WORLD_DEPTH; k++)
			{
				if (i > 0)
					chunks[i][j][k]->cXN = chunks[i - 1][j][k];
				if (i < WORLD_WIDTH - 1)
					chunks[i][j][k]->cXP = chunks[i + 1][j][k];
				if (j > 0)
					chunks[i][j][k]->cYN = chunks[i][j - 1][k];
				if (j < WORLD_HEIGHT - 1)
					chunks[i][j][k]->cYP = chunks[i][j + 1][k];
				if (k > 0)
					chunks[i][j][k]->cZN = chunks[i][j][k - 1];
				if (k < WORLD_WIDTH - 1)
					chunks[i][j][k]->cZP = chunks[i][j][k + 1];
			}
	update();
}

World::~World()
{
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			delete[] chunks[i][j];
		}
		delete[] chunks[i];
	}
	delete[] chunks;
}

void World::update()
{
	for (int i = 0; i < WORLD_WIDTH; i++)
		for (int j = 0; j < WORLD_HEIGHT; j++)
			for (int k = 0; k < WORLD_DEPTH; k++)
				updateChunkBlockFaces(chunks[i][j][k]);
	updateChunkFacesToRender(this);
}

void updateChunkFacesToRender(World* world)
{
	bool bDefault = true;
	for (int i = 0; i < WORLD_WIDTH; i++)	// change range to -WORLD_WIDTH/2 -> WORLD_WIDTH/2
	{
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			for (int k = 0; k < WORLD_DEPTH; k++)
			{
				Chunk* c = world->chunks[i][j][k];

				// if neighbouring chunk is active, don't render face
				if (c->cXN != NULL)
					c->removeFaceFromRender(FaceDirection::XNeg);
				if (c->cXP != NULL)
					c->removeFaceFromRender(FaceDirection::XPos);
				if (c->cYN != NULL)
					c->removeFaceFromRender(FaceDirection::YNeg);
				if (c->cYP != NULL)
					c->removeFaceFromRender(FaceDirection::YPos);
				if (c->cZN != NULL)
					c->removeFaceFromRender(FaceDirection::ZNeg);
				if (c->cZP != NULL)
					c->removeFaceFromRender(FaceDirection::ZPos);
			}
		} 
	}
}

void updateChunkOnModify(World* world, Chunk* chunk)
{
	updateChunkBlockFaces(chunk);
	updateChunkFacesToRender(world);
}
