#include "world.h"

World::World()
{
	chunks = new Chunk * * * [WORLD_WIDTH];
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		chunks[i] = new Chunk * * [WORLD_HEIGHT];
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			chunks[i][j] = new Chunk * [WORLD_DEPTH];
		}
	}

	for (int i = 0; i < WORLD_WIDTH; i++)
		for (int j = 0; j < WORLD_HEIGHT; j++)
			for (int k = 0; k < WORLD_DEPTH; k++)
				chunks[i][j][k] = new Chunk(this, i, j - WORLD_HEIGHT, k);

	//updateChunkFacesToRender(this);
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
	{
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			for (int k = 0; k < WORLD_DEPTH; k++)
			{
				updateChunkBlockFaces(chunks[i][j][k]);
			}
		}
	}
}

void updateChunkFacesToRender(World* world)
{
	bool bDefault = true;
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			for (int k = 0; k < WORLD_DEPTH; k++)
			{
				bool Xn = bDefault;
				bool Xp = bDefault;
				bool Yn = bDefault;
				bool Yp = bDefault;
				bool Zn = bDefault;
				bool Zp = bDefault;
				//if (i > 0)
				//{
				//	Xn = world->chunks[i - i][j][k]->isEmpty;
				//}
				//if (i < CHUNK_SIZE - 1)
				//{
				//	Xp = !chunk->data[to_data_index(i + 1, j, k)]->isActive;
				//}
				//if (j > 0)
				//{
				//	Yn = !chunk->data[to_data_index(i, j - 1, k)]->isActive;
				//}
				//if (j < CHUNK_SIZE - 1)
				//{
				//	Yp = !chunk->data[to_data_index(i, j + 1, k)]->isActive;
				//}
				//if (k > 0)
				//{
				//	Zn = !chunk->data[to_data_index(i, j, k - 1)]->isActive;
				//}
				//if (k < CHUNK_SIZE - 1)
				//{
				//	Zp = !chunk->data[to_data_index(i, j, k + 1)]->isActive;
				//}
			}
		} 
	}
}

void updateChunkOnModify(World* world, Chunk* chunk)
{
	updateChunkBlockFaces(chunk);
	updateChunkFacesToRender(world);
}
