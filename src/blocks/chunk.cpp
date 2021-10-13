#include "chunk.h"
#include "../graphics/window/window.h"
#include "../world/world.h"

Chunk::Chunk()
{
}

// Initialize a chunk's **data
Chunk::Chunk(int i, int j, int k)
{
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
				delete getBlockFromIndex(i, j, k);
	if (data != NULL)
		delete[] data;
}

void Chunk::generateChunkData()
{
	int airCount = 0;
	BlockType type;
	int bX = chunkX, bY = chunkY, bZ = chunkZ;
	for (int i = 0; i < CHUNK_SIZE; i++)
	{
		for (int j = 0; j < CHUNK_SIZE; j++)
		{
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				int access = to_data_index(i, j, k);
				//int height = (int)to_global_coord(i, j, k).y;
				//if (height < -2)
				//	type = BlockType::Stone;
				//else
				type = BlockType::Grass;
				//if (type == BlockType::Grass)
				//	airCount++;
				data[access] = new Block((float)(chunkX + i), (float)(chunkY + j), (float)(chunkZ + k), 1.0f, type);
			}
		}
	}
	if (airCount == CHUNK_SIZE_CUBED)	// maybe put this in the update chunk block face function so you check every time
		isEmpty = true;
	updateChunkBlockFaces(this);
}

// Removes the face of a chunk by unrendering 
void Chunk::removeChunkFaceFromRender(FaceDirection face)
{
	switch (face)
	{
	case FaceDirection::XNeg:
		for (int j = 0; j < CHUNK_SIZE; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
				get_block_from_index(this, 0, j, k)->renderFace[3] = false;
		break;
	case FaceDirection::XPos:
		for (int j = 0; j < CHUNK_SIZE; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
				get_block_from_index(this, CHUNK_SIZE - 1, j, k)->renderFace[2] = false;
		break;
	case FaceDirection::YNeg:
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int k = 0; k < CHUNK_SIZE; k++)
				get_block_from_index(this, i, 0, k)->renderFace[5] = false;
		break;
	case FaceDirection::YPos:
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int k = 0; k < CHUNK_SIZE; k++)
				get_block_from_index(this, i, CHUNK_SIZE - 1, k)->renderFace[4] = false;
		break;
	case FaceDirection::ZNeg:
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_SIZE; j++)
				get_block_from_index(this, i, j, 0)->renderFace[0] = false;
		break;
	case FaceDirection::ZPos:
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_SIZE; j++)
				get_block_from_index(this, i, j, CHUNK_SIZE - 1)->renderFace[1] = false;
		break;
	}
}

// Returns the block in the chunk at the zero-based position (i, j, k)
Block* Chunk::getBlockFromIndex(int i, int j, int k)
{
	i = INDEX_INBOUNDS(i);
	j = INDEX_INBOUNDS(j);
	k = INDEX_INBOUNDS(k);
	return data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED];
}

const glm::vec3 Chunk::to_global_coord(int i, int j, int k)
{
	return glm::vec3(chunkX + i, chunkY + j, chunkZ - k);
}

const glm::vec3 world_pos_to_chunk_block_coord(Chunk* c, const glm::vec3& pos)
{
	// block position within chunk c
	int bX = (int)glm::floor(pos.x - (float)c->chunkX);
	int bY = (int)glm::floor(pos.y - (float)c->chunkY);
	int bZ = (int)glm::floor(pos.z - (float)c->chunkZ);
	return glm::vec3(bX, bY, bZ);
}

Block* world_pos_to_chunk_block(Chunk* c, const glm::vec3& pos)
{
	glm::vec3 blockPos= world_pos_to_chunk_block_coord(c, pos);
	return get_block_from_index(c, (int)blockPos.x, (int)blockPos.y, (int)blockPos.z);
}

Block* get_block_from_index(Chunk* chunk, int i, int j, int k)
{
	return chunk->data[to_data_index(i, j, k)];
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
	int xNFaceCount = 0;
	int xPFaceCount = 0;
	int yNFaceCount = 0;
	int yPFaceCount = 0;
	int zNFaceCount = 0;
	int zPFaceCount = 0;
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
				Block* b;
				if (i > 0)
					Xn = !chunk->data[to_data_index(i - 1, j, k)]->isActive;
				else if (i == 0 && chunk->cXN != NULL) // edge of chunk, compare with nearby chunk block
				{
					b = chunk->cXN->data[to_data_index(CHUNK_SIZE - 1, j, k)];
					Xn = !chunk->cXN->data[to_data_index(CHUNK_SIZE - 1, j, k)]->isActive;
				}

				if (i < CHUNK_SIZE - 1)
					Xp = !chunk->data[to_data_index(i + 1, j, k)]->isActive;
				else if (i == CHUNK_SIZE - 1 && chunk->cXP != NULL)
				{
					b = chunk->cXP->data[to_data_index(0, j, k)];
					Xp = !chunk->cXP->data[to_data_index(0, j, k)]->isActive;
				}

				if (j > 0)
					Yn = !chunk->data[to_data_index(i, j - 1, k)]->isActive;
				else if (j == 0 && chunk->cYN != NULL)
				{
					b = chunk->cYN->data[to_data_index(i, CHUNK_SIZE - 1, k)];
					Yn = !chunk->cYN->data[to_data_index(i, CHUNK_SIZE - 1, k)]->isActive;
				}

				if (j < CHUNK_SIZE - 1)
					Yp = !chunk->data[to_data_index(i, j + 1, k)]->isActive;
				else if (j == CHUNK_SIZE - 1 && chunk->cYP != NULL)
				{
					b = chunk->cYP->data[to_data_index(i, 0, k)];
					Yp = !chunk->cYP->data[to_data_index(i, 0, k)]->isActive;
				}


				if (k > 0)
					Zn = !chunk->data[to_data_index(i, j, k - 1)]->isActive;
				else if (k == 0 && chunk->cZN != NULL)
				{
					b = chunk->cZN->data[to_data_index(i, j, CHUNK_SIZE - 1)];
					Zn = !chunk->cZN->data[to_data_index(i, j, CHUNK_SIZE - 1)]->isActive;
				}

				if (k < CHUNK_SIZE - 1)
					Zp = !chunk->data[to_data_index(i, j, k + 1)]->isActive;
				else if (k == CHUNK_SIZE - 1 && chunk->cZP != NULL)
				{
					b = chunk->cZP->data[to_data_index(i, j, 0)];
					Zp = !chunk->cZP->data[to_data_index(i, j, 0)]->isActive;
				}

				chunk->data[access]->setFacesToRender(Xn, Xp, Yn, Yp, Zn, Zp);
			}
		}
	}
}

void updateChunkFaces(Chunk* c)
{
	// if neighbouring chunk is active, don't render face **TEMPORARY**
	int faceCountXN = CHUNK_SIZE;
	int faceCountXP = CHUNK_SIZE;
	int faceCountYN = CHUNK_SIZE;
	int faceCountYP = CHUNK_SIZE;
	int faceCountZN = CHUNK_SIZE;
	int faceCountZP = CHUNK_SIZE;

	// if yes, face is full so don't render the face
	if (c->cXN != NULL)
		c->removeChunkFaceFromRender(FaceDirection::XNeg);
	if (c->cXP != NULL)
		c->removeChunkFaceFromRender(FaceDirection::XPos);
	if (c->cYN != NULL)
		c->removeChunkFaceFromRender(FaceDirection::YNeg);
	if (c->cYP != NULL)
		c->removeChunkFaceFromRender(FaceDirection::YPos);
	if (c->cZN != NULL)
		c->removeChunkFaceFromRender(FaceDirection::ZNeg);
	if (c->cZP != NULL) 
		c->removeChunkFaceFromRender(FaceDirection::ZPos);

	// decide which block faces to render within chunk
	updateChunkBlockFaces(c);

	c->isModified = false;
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