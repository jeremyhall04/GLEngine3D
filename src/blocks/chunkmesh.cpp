#include "chunkmesh.h"
#include "chunk.h"

ChunkMesh::ChunkMesh()
{
}

ChunkMesh::ChunkMesh(Chunk* chunk)
	: chunk(chunk)
{
	const int nVertices = CHUNK_SIZE_CUBED * 36;
	meshSize = (size_t)(sizeof(VertexData) * nVertices);
	meshData = new VertexData[nVertices];	// give world a mapped buffer object that points to the chunks vbo's

	generateMesh();

	vao.genBufferData(&vbo, meshSize, sizeof(VertexData), meshData);
	chunk->mesh = this;
}

ChunkMesh::~ChunkMesh()
{
	delete[] meshData;
	meshData = NULL;
}

void ChunkMesh::generateMesh()
{
	index = 0;
	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_SIZE; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				//if (chunk->data[to_data_index(i, j, k)]->isActive)
				//{
					generateBlockMesh(i, j, k);
					index += 36;
				//}
			}
}

void ChunkMesh::updateMesh()	// TODO: NOT WORKING PROPERLY, top of block is not regenerating properly
{
	// clear meshData
	memset(meshData, 0, meshSize);

	// generate new meshData
	generateMesh();

	// fill vbo
	vbo.bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, meshSize, meshData);
	vbo.unbind();
}


// TODO: This creates a block mesh the same size, even if only 1 face is showing & only 6 vertices should be used.
void ChunkMesh::generateBlockMesh(int i, int j, int k)
{
	int access = to_data_index(i, j, k);
	glm::vec3 blockOffset(chunk->chunkX + i, chunk->chunkY + j, chunk->chunkZ + k);
	BlockType type = chunk->data[access]->getType();

	// generate triangle mesh for current block
	int faceCount = 0;
	int w = 0; // w is the number of actual data points written into meshData
	if (type != BlockType::Grass)
	{
		for (int v = 0; v < 36; v++)
		{
			if ((v == 6) || (v == 12) || (v == 18) || (v == 24) || (v == 30) || (v == 36))
				faceCount++;
			if (chunk->data[access]->renderFace[faceCount] == false)
			{
				v += 5;
				continue;
			}
			const float* pos = &BLOCK_VERTICES[v * 3];
			const float* uv = &BLOCK_UV[v * 2];
			meshData[index + v].position = glm::vec3(blockOffset.x + pos[0], blockOffset.y + pos[1], blockOffset.z + pos[2]);
			meshData[index + v].uv = glm::vec2(uv[0], uv[1]);
			meshData[index + v].tid = (float)type;
			//w++;
			//meshData[index * 36 + v].light = ;
		}
	}
	else
	{
		for (int v = 0; v < 36; v++)
		{
			if ((v == 6) || (v == 12) || (v == 18) || (v == 24) || (v == 30) || (v == 36))
				faceCount++;
			if (chunk->data[access]->renderFace[faceCount] == false)
			{
				v += 5;
				continue;
			}
			const float* pos = &BLOCK_VERTICES[v * 3];
			const float* uv = &BLOCK_UV[v * 2];
			meshData[index + v].position = glm::vec3(blockOffset.x + pos[0], blockOffset.y + pos[1], blockOffset.z + pos[2]);
			meshData[index + v].uv = glm::vec2(uv[0], uv[1]);
			if (faceCount == 4)
				meshData[index + v].tid = 4;	// grass (top)
			else if (faceCount == 5)
				meshData[index + v].tid = 2;	// dirt (bottom)
			else
				meshData[index + v].tid = 3;
			//w++;
			//meshData[index * 36 + v].light = ;
		}
	}
}

void renderChunkMesh(ChunkMesh* mesh)
{
	glBindVertexArray(mesh->vao.handle);
	glDrawArrays(GL_TRIANGLES, 0, mesh->index);
}