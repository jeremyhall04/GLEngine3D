#include "world.h"
#include "../entity/player.h"

World::World()
{
}

World::World(Player* player)
	: player(player)
{
	initialize();
	updateWorldFirstPass();
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

void World::initialize()
{
	chunks = new Chunk * * *[WORLD_WIDTH];
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		chunks[i] = new Chunk * *[WORLD_HEIGHT];
		for (int j = 0; j < WORLD_HEIGHT; j++)
			chunks[i][j] = new Chunk * [WORLD_DEPTH];
	}

	for (int i = 0; i < WORLD_WIDTH; i++)
		for (int j = 0; j < WORLD_HEIGHT; j++)
			for (int k = 0; k < WORLD_DEPTH; k++)
				chunks[i][j][k] = new Chunk(i, j, k);

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
}

void World::update()
{
	// figure out which chunk the block to remove is
	if (blockRemove)
	{
		// cast ray and remove block
		for (float step = 0.0f; step < player->ray.maxRayLength; step += 0.5f)
		{
			player->ray.update(player->getPos(), player->getDir(), step);

			float rx = player->ray.endPoint.x;
			float ry = player->ray.endPoint.y;
			float rz = player->ray.endPoint.z;
			int cX = (int)glm::floor(rx / CHUNK_SIZE);
			int cY = (int)glm::floor(ry / (WORLD_HEIGHT * CHUNK_SIZE));
			int cZ = (int)glm::floor(rz / CHUNK_SIZE);

			// therefore, looking at block in chunk c
			if (cX < 0 || cX >= (WORLD_WIDTH) || cY != 0 || cZ < 0 || cZ >= (WORLD_WIDTH))
				//printf("\nchunk[%d][%d][%d] out of range", cX, cY, cZ);
				blockRemove = false;
			else
			{
				// update chunk
				Chunk* c = chunks[cX][cY][cZ];
				if (c != NULL)
				{
					// add chunk to update list
					chunkUpdateList = c;
					if (chunkUpdateListCount == 0)
						chunkUpdateListStart = chunkUpdateList;	// remember starting memory address
					chunkUpdateList++;		// move pointer to next chunk in list
					chunkUpdateListCount++;	// increment count 

					// remove block within chunk
					int bX = (int)glm::floor(rx - (float)c->chunkX);
					int bY = (int)glm::floor(ry - (float)c->chunkY);
					int bZ = (int)glm::floor(rz - (float)c->chunkZ);
					if (bY <= 15)
					{
						int access = to_data_index(bX, bY, bZ);
						Block* b = c->data[access];
						if (b->isActive == false)
							continue;
						else
						{
							b->isActive = false;
							blockRemove = false;
							c->isModified = true;
							//printf("\nRemoved C[%d][%d][%d].B[%d][%d][%d]", cX, cY, cZ, bX, bY, bZ);
							// if block is on edge of chunk, update neighbouring chunk
							if (bX <= 0 && c->cXN != NULL)
							{
								c->cXN->isModified = true;
								*chunkUpdateList = *(c->cXN);
								chunkUpdateList++;
								chunkUpdateListCount++;
								break;
							}
							else if (bX >= 15 && c->cXP != NULL)
							{
								//printf("\n\tis border, to Render C[%d][%d][%d].B[%d][%d][%d]", cX + 1, cY, cZ, 0, bY, bZ);
								c->cXP->isModified = true;
								*chunkUpdateList = *(c->cXP);
								chunkUpdateList++;
								chunkUpdateListCount++;
								//addBlockToChunkRender(c->cXP, glm::vec3(bX, bY, bZ), chunkUpdateList, chunkUpdateListCount);
								break;
							}
							if (bZ <= 0 && c->cZN != NULL)
							{
								c->cZN->isModified = true;
								*chunkUpdateList = *(c->cZN);
								chunkUpdateList++;
								chunkUpdateListCount++;
								break;
							}
							else if (bZ >= 15 && c->cZP != NULL)
							{
								c->cZP->isModified = true;
								*chunkUpdateList = *(c->cZP);
								chunkUpdateList++;
								chunkUpdateListCount++;
								break;
							}

							break;
						}
					}
					else
						printf("\nchunk[%d][%d][%d].block[%d][%d][%d] is out of bounds", cX, cY, cZ, bX, bY, bZ);
				}
				else
					printf("\nchunk[%d][%d][%d] was NULL", cX, cY, cZ);
			}
		}
	}
	
	if (!chunkUpdateListCount == 0)
	{
		chunkUpdateList = chunkUpdateListStart;
		for (int i = 0; i < chunkUpdateListCount; i++)
		{
			Chunk* c = chunkUpdateList;
			if (c->isModified)
				updateChunkFaces(c);
			chunkUpdateList++;
		}
		chunkUpdateListCount = 0;
	}

	//for (int i = 0; i < WORLD_WIDTH; i++)
	//	for (int j = 0; j < WORLD_HEIGHT; j++)
	//		for (int k = 0; k < WORLD_DEPTH; k++)
	//		{
	//			// check if player is looking at block (& in range)
	//			if (!chunks[i][j][k]->isEmpty && chunks[i][j][k]->isModified)
	//			{
	//				updateChunkBlockFaces(chunks[i][j][k]);
	//				updateChunkFaces(chunks[i][j][k]);
	//				chunks[i][j][k]->isModified = false;
	//			}
	//		}
}



void World::mouseEvent(int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		blockRemove = true;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		blockRemove = false;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		blockPlace = true;
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		blockPlace = false;
}

void World::updateWorldFirstPass()
{
	for (int i = 0; i < WORLD_WIDTH; i++)
		for (int j = 0; j < WORLD_HEIGHT; j++)
			for (int k = 0; k < WORLD_DEPTH; k++)
			{
				Chunk* c = chunks[i][j][k];
				// check if player is looking at block (& in range)
				if (!c->isEmpty)
				{
					updateChunkBlockFaces(c);
					updateChunkFaces(c);
				}
				else
					c = NULL;
			}
	
}

const glm::vec3 pos_to_world_chunk_coord(float x, float y, float z)
{
	glm::vec3 result(0.0f);
	int cX = (int)glm::floor(x / WORLD_WIDTH);
	int cY = (int)glm::floor(y / (WORLD_HEIGHT * CHUNK_SIZE));
	int cZ = (int)glm::floor(z / WORLD_DEPTH);
	return glm::vec3(cX, cY, cZ);
}

const glm::vec3 pos_to_world_chunk_coord(const glm::vec3& pos)
{
	return pos_to_world_chunk_coord(pos.x, pos.y, pos.z);
}

void pos_to_world_chunk(World* world, const glm::vec3& pos, Chunk* c)
{
	glm::vec3 p = pos_to_world_chunk_coord(pos);
	if (p.x < 0 || p.y != 0 || p.z < 0)
	{
		printf("\nchunk[%d][%d][%d] out of range", (int)p.x, (int)p.y, (int)p.z);
		c = NULL;
	}
	else
		c = world->chunks[(int)p.x][(int)p.y][(int)p.z];
}