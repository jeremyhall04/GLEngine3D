#include "chunk.h"
#include "../graphics/window/window.h"
#include "../world/world.h"

Chunk::Chunk()
{
}

// Initialize a chunk's **data
Chunk::Chunk(World* world, int i, int j, int k)
{
	this->world = world;
	data = new Block * [CHUNK_SIZE_CUBED];
	chunkX = i * CHUNK_SIZE;
	chunkY = j * CHUNK_SIZE;
	chunkZ = k * CHUNK_SIZE;
	generateChunkData();
}

Chunk::~Chunk()
{
	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_SIZE; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
				delete get_block_from_pos_in_chunk(i, j, k);
	if (data != NULL)
		delete[] data;
}

void Chunk::generateChunkData()
{
	BlockType type;
	int bX = chunkX, bY = chunkY, bZ = chunkZ;
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				int access = to_data_index(i, j, k);
				int height = to_global_coord(i, j, k).y;
				if (height < -2)
					type = BlockType::Stone;
				else
					type = BlockType::Grass;
				data[access] = new Block((float)(chunkX + i), (float)(chunkY + j), (float)(chunkZ + k), 1.0f, type);
			}
		}
	}
	updateChunkBlockFaces(this);
}

// Returns the block in the chunk at the zero-based position (i, j, k)
Block* Chunk::get_block_from_pos_in_chunk(int i, int j, int k)
{
	i = INDEX_INBOUNDS(i);
	j = INDEX_INBOUNDS(j);
	k = INDEX_INBOUNDS(k);
	return data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED];
}

const glm::vec3& Chunk::to_global_coord(int i, int j, int k)
{
	return glm::vec3(chunkX + i, chunkY + j, chunkZ - k);
}

int to_data_index(int i, int j, int k)
{
	i = INDEX_INBOUNDS(i);
	j = INDEX_INBOUNDS(j);
	k = INDEX_INBOUNDS(k);
	int access = i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED;
	return access;
}



void updateChunkBlockFaces(Chunk* chunk)
{
	// tell the block which faces are hidden
	bool bDefault = true;
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				int access = to_data_index(i, j, k);
				bool Xn = bDefault;
				bool Xp = bDefault;
				bool Yn = bDefault;
				bool Yp = bDefault;
				bool Zn = bDefault;
				bool Zp = bDefault;
				if (i > 0)
				{
					Xn = !chunk->data[to_data_index(i - 1, j, k)]->isActive;
				}
				if (i < CHUNK_SIZE - 1)
				{
					Xp = !chunk->data[to_data_index(i + 1, j, k)]->isActive;
				}
				if (j > 0)
				{
					Yn = !chunk->data[to_data_index(i, j - 1, k)]->isActive;
				}
				if (j < CHUNK_SIZE - 1)
				{
					Yp = !chunk->data[to_data_index(i, j + 1, k)]->isActive;
				}
				if (k > 0)
				{
					Zn = !chunk->data[to_data_index(i, j, k - 1)]->isActive;
				}
				if (k < CHUNK_SIZE - 1)
				{
					Zp = !chunk->data[to_data_index(i, j, k + 1)]->isActive;
				}
				chunk->data[access]->setFacesToRender(Xn, Xp, Yn, Yp, Zn, Zp);
			}
		}
	}
}

//void updateChunkFaces(World* world)
//{
//	// tell the block which faces are hidden
//	bool bDefault = true;
//	for (int i = 0; i < CHUNK_SIZE; i++)
//	{
//		for (int j = 0; j < CHUNK_SIZE; j++)
//		{
//			for (int k = 0; k < CHUNK_SIZE; k++)
//			{
//				int access = to_data_index(i, j, k);
//				bool Xn = bDefault;
//				bool Xp = bDefault;
//				bool Yn = bDefault;
//				bool Yp = bDefault;
//				bool Zn = bDefault;
//				bool Zp = bDefault;
//				if (i > 0)
//				{
//					Xn = !chunk->data[to_data_index(i - 1, j, k)]->isActive;
//				}
//				if (i < CHUNK_SIZE - 1)
//				{
//					Xp = !chunk->data[to_data_index(i + 1, j, k)]->isActive;
//				}
//				if (j > 0)
//				{
//					Yn = !chunk->data[to_data_index(i, j - 1, k)]->isActive;
//				}
//				if (j < CHUNK_SIZE - 1)
//				{
//					Yp = !chunk->data[to_data_index(i, j + 1, k)]->isActive;
//				}
//				if (k > 0)
//				{
//					Zn = !chunk->data[to_data_index(i, j, k - 1)]->isActive;
//				}
//				if (k < CHUNK_SIZE - 1)
//				{
//					Zp = !chunk->data[to_data_index(i, j, k + 1)]->isActive;
//				}
//				chunk->data[access]->setFacesToRender(Xn, Xp, Yn, Yp, Zn, Zp);
//			}
//		}
//	}
//}

//void Chunk::generateMesh(ChunkHelper helper)
//{
//	cXN = &(world->chunks[chunkX - 1][chunkY][chunkZ]);
//	cXP = &(world->chunks[chunkX + 1][chunkY][chunkZ]);
//	cYN = &(world->chunks[chunkX][chunkY - 1][chunkZ]);
//	cYP = &(world->chunks[chunkX][chunkY + 1][chunkZ]);
//	cZN = &(world->chunks[chunkX][chunkY][chunkZ - 1]);
//	cZP = &(world->chunks[chunkX][chunkY][chunkZ + 1]);
//
//	// start at chunkpos 0,0,0
//	// Y axis
//	for (int j = 0; j < CHUNK_SIZE; j++)
//	{
//		// Z axis
//		for (int k = 0; k < CHUNK_SIZE; k++)
//		{
//			// X axis
//			for (int i = 0; i < CHUNK_SIZE; i++)
//			{
//				int access = i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED;
//				Block* b = data[access];
//				if (b->isActive())
//					createRuns(b, i, j, k, helper, access);
//			}
//		}
//	}
//}

//void Chunk::createRuns(Block* block, int i, int j, int k, ChunkHelper& helper, int access)
//{
//	int i1 = i + 1;
//	int j1 = j + 1;
//	int k1 = k + 1;
//
//	int length = 0;
//	int chunkAccess = 0;
//
//	if (!helper.visitedXN[access])
//	{
//		for (int q = j; q < CHUNK_SIZE; q++)
//		{
//			chunkAccess = i + q * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED;
//			//if ()
//		}
//	}
//}
//
//bool Chunk::visibleFaceXN(int i, int j, int k)
//{
//	// Access directly from a neighbouring chunk
//	bool isVisible = true;
//	if (i < 0)
//	{
//		if (cXN == NULL)
//			isVisible = true;
//		else
//			isVisible = cXN->data[31 + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED]->isActive();
//	}
//	else
//		isVisible = cXN->data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED]->isActive();
//	return isVisible;
//} 
//
//bool Chunk::visibleFaceXP(int i, int j, int k)
//{
//	bool isVisible = true;
//	if (i >= CHUNK_SIZE)
//	{
//		if (cXP == NULL)
//			isVisible = true;
//		else
//			isVisible = cXN->data[0 + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED]->isActive();
//	}
//	else
//		isVisible = cXN->data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED]->isActive();
//	return isVisible;
//}