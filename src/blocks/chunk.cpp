#include "chunk.h"
#include "../graphics/window/window.h"
#include "../world/world.h"

#include "../utils/crtdebug.h"

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
	if (data != NULL)
	{
		for (int i = 0; i < CHUNK_SIZE; i++)
			for (int j = 0; j < CHUNK_SIZE; j++)
				for (int k = 0; k < CHUNK_SIZE; k++)
				{
					delete data[to_data_index(i, j, k)];	// deleting BLOCKS in chunk
					data[to_data_index(i, j, k)] = NULL;	// setting BLOCK to null
				}
		delete[] data;	// deleting block array
		data = NULL;

		cXN = NULL;
		cXP = NULL;
		cYN = NULL;
		cYP = NULL;
		cZN = NULL;
		cZP = NULL;
	}
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
				int height = (int)to_global_coord(i, j, k).y;
				if (height < 10)
					type = BlockType::Stone;
				else if (height > 32)
				{
					type = BlockType::Air;
					airCount++;
				}
				else if (j + 1 == CHUNK_SIZE)
					type = BlockType::Grass;
				else
					type = BlockType::Dirt;
				//if (type == BlockType::Grass)
				//	airCount++;
				data[access] = new Block((float)(chunkX + i), (float)(chunkY + j), (float)(chunkZ + k), 1.0f, type);
			}
		}
	}
	if (airCount == CHUNK_SIZE_CUBED)	// maybe put this in the update chunk block face function so you check every time
		isEmpty = true;
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

void index_inbounds(int& x)
{
	x = (x < 0) ? 0 : x;
	x = (x > CHUNK_SIZE - 1) ? CHUNK_SIZE - 1 : x;
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
	index_inbounds(i);
	index_inbounds(j);
	index_inbounds(k);
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
				Block* b = chunk->data[to_data_index(i, j, k)];
				if (!b->isActive)
				{
					chunk->data[access]->setFacesToRender(false, false, false, false, false, false);
					continue;
				}
				bool Xn = bDefault;
				bool Xp = bDefault;
				bool Yn = bDefault;
				bool Yp = bDefault;
				bool Zn = bDefault;
				bool Zp = bDefault;
				if (i > 0)
					Xn = !chunk->data[to_data_index(i - 1, j, k)]->isActive;
				else if (i == 0 && chunk->cXN != NULL) // edge of chunk, compare with nearby chunk block
				{
					//b = chunk->cXN->data[to_data_index(CHUNK_SIZE - 1, j, k)];
					Xn = !chunk->cXN->data[to_data_index(CHUNK_SIZE - 1, j, k)]->isActive;
				}					

				if (i < CHUNK_SIZE - 1)
					Xp = !chunk->data[to_data_index(i + 1, j, k)]->isActive;
				else if (i == CHUNK_SIZE - 1 && chunk->cXP != NULL)
				{
					//b = chunk->cXP->data[to_data_index(0, j, k)];
					Xp = !chunk->cXP->data[to_data_index(0, j, k)]->isActive;
				}

				if (j > 0)
					Yn = !chunk->data[to_data_index(i, j - 1, k)]->isActive;
				else if (j == 0 && chunk->cYN != NULL)
				{
					//b = chunk->cYN->data[to_data_index(i, CHUNK_SIZE - 1, k)];
					Yn = !chunk->cYN->data[to_data_index(i, CHUNK_SIZE - 1, k)]->isActive;
				}

				if (j < CHUNK_SIZE - 1)
					Yp = !chunk->data[to_data_index(i, j + 1, k)]->isActive;
				else if (j == CHUNK_SIZE - 1 && chunk->cYP != NULL)
				{
					//b = chunk->cYP->data[to_data_index(i, 0, k)];
					Yp = !chunk->cYP->data[to_data_index(i, 0, k)]->isActive;
				}


				if (k > 0)
					Zn = !chunk->data[to_data_index(i, j, k - 1)]->isActive;
				else if (k == 0 && chunk->cZN != NULL)
				{
					//b = chunk->cZN->data[to_data_index(i, j, CHUNK_SIZE - 1)];
					Zn = !chunk->cZN->data[to_data_index(i, j, CHUNK_SIZE - 1)]->isActive;
				}

				if (k < CHUNK_SIZE - 1)
					Zp = !chunk->data[to_data_index(i, j, k + 1)]->isActive;
				else if (k == CHUNK_SIZE - 1 && chunk->cZP != NULL)
				{
					//b = chunk->cZP->data[to_data_index(i, j, 0)];
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