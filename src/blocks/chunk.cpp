#include "chunk.h"
#include "../graphics/window/window.h"

Chunk::Chunk(float offsetX, float offsetY, float offsetZ)
{
	offset.x = offsetX;
	offset.y = offsetY;
	offset.z = offsetZ;
}

Chunk::~Chunk()
{
	for (int i = 0; i < CHUNK_SIZE_MAX; i++) 
	{
		for (int j = 0; j < CHUNK_SIZE_MAX; j++) 
		{
			delete[] chunkBlocks[i][j];
		}
		delete[] chunkBlocks[i];
	}
	delete[] chunkBlocks;
}

void Chunk::generateChunk(BlockType type)
{
	float bX = this->offset.x, bY = this->offset.y, bZ = this->offset.z;
	float blockSize = 1.0f;
	
	chunkBlocks = new Block * *[CHUNK_SIZE_MAX];
	for (int i = 0; i < CHUNK_SIZE_MAX; i++) {
		chunkBlocks[i] = new Block * [CHUNK_SIZE_MAX];
		for (int j = 0; j < CHUNK_SIZE_MAX; j++) {
			chunkBlocks[i][j] = new Block[CHUNK_SIZE_MAX];
		}
	}

	for (int j = 0; j < CHUNK_SIZE_MAX; j++) {
		bY = j * blockSize;
		for (int k = 0; k < CHUNK_SIZE_MAX; k++) {
			bZ = k * blockSize;
			for (int i = 0; i < CHUNK_SIZE_MAX; i++) {
				bX = i * blockSize;
				Block* newCube = new Block(bX, bY, -bZ, blockSize, type);
				chunkBlocks[i][j][k] = *newCube;
			}
		}
	}
	isGenerated = true;
	blockCount = CHUNK_VOLUME;
}

void occlusionCull(Chunk* chunk, glm::vec3 position)
{
	glm::vec3 camDir = g_CameraPtr->getDirection();
	glm::vec3 camPos = g_CameraPtr->getPosition();
	glm::mat4 projection = g_CameraPtr->getProjectionMatrix();
	glm::mat4 view = g_CameraPtr->getViewMatrix();
}

void frustumCull(Chunk* chunk)
{
	const glm::vec4 params = g_CameraPtr->getPerspectiveParams();
	glm::mat4 P = g_CameraPtr->getProjectionMatrix();
	glm::vec3 camDir = g_CameraPtr->getDirection();
	glm::vec3 camPos = g_CameraPtr->getPosition();
	glm::vec3 camUp = g_CameraPtr->getUp();
	glm::vec3 camRight = g_CameraPtr->getRight();
	float fov = params.x;
	float aspect = params.y;
	float zNear = params.z;
	float zFar = params.w;
	float hNear = 2.0f * tan(fov / 2.0f) * zNear;
	float wNear = hNear * aspect;
	float hFar = 2.0f * tan(fov / 2.0f) * zFar;
	float wFar = hFar * aspect;

	glm::vec3 fc = camPos + camDir * zFar;
	glm::vec3 ftl = fc + (camUp * hFar / 2.0f) - (camRight * wFar / 2.0f);
	glm::vec3 ftr = fc + (camUp * hFar / 2.0f) + (camRight * wFar / 2.0f);
	glm::vec3 fbl = fc - (camUp * hFar / 2.0f) - (camRight * wFar / 2.0f);
	glm::vec3 fbr = fc - (camUp * hFar / 2.0f) + (camRight * wFar / 2.0f);

	glm::vec3 nc = camPos + camDir * zNear;
	glm::vec3 ntl = fc + (camUp * hNear / 2.0f) - (camRight * wNear / 2.0f);
	glm::vec3 ntr = fc + (camUp * hNear / 2.0f) + (camRight * wNear / 2.0f);
	glm::vec3 nbl = fc - (camUp * hNear / 2.0f) - (camRight * wNear / 2.0f);
	glm::vec3 nbr = fc - (camUp * hNear / 2.0f) + (camRight * wNear / 2.0f);

	glm::vec3 a = (nc + camRight * wNear / 2.0f) - camPos;
	a = glm::normalize(a);
	glm::vec3 normalRight = camUp * a;

	GLuint count = 0;
	for (GLuint i = 0; i < CHUNK_SIZE_X; i++)
	{
		for (GLuint j = 0; j < CHUNK_SIZE_Y; j++)
		{
			for (GLuint k = 0; k < CHUNK_SIZE_Z; k++)
			{
				bool inside = false;
				Block* block = get_block_from_position(chunk, glm::vec3(i, j, k));
				glm::vec4 normalizedV = glm::normalize(glm::vec4(block->getPosition(), 1.0f));
				float x = normalizedV.x;
				float y = normalizedV.y;
				float z = normalizedV.z;

				glm::vec3 vertices[8] = {
					glm::vec3(x,						y,							z),
					glm::vec3(x,						y + block->getSize().y,		z),
					glm::vec3(x + block->getSize().x,	y + block->getSize().y,		z),
					glm::vec3(x + block->getSize().x,	y,							z),
					glm::vec3(x,						y,							z + block->getSize().z),
					glm::vec3(x,						y + block->getSize().y,		z + block->getSize().z),
					glm::vec3(x + block->getSize().x,	y + block->getSize().y,		z + block->getSize().z),
					glm::vec3(x + block->getSize().x,	y,							z + block->getSize().z)
				};

				for (GLuint v = 0; v < 8; v++)
				{
					bool isX = false, isY = false, isZ = false;

					isX = (-1.0f < vertices[v].x && vertices[v].x < 1.0f);	// true if inside
					isY = (-1.0f < vertices[v].y && vertices[v].y < 1.0f);
					isZ = (-1.0f < vertices[v].z && vertices[v].z < 1.0f);
					inside = isX || isY || isZ;	// if any point is inside, render the block
				}
				block->setActive(inside);
				if (inside)
					count++;
			}
		}
	}
	if (count == 0)
		chunk->isRender = false;
}

Block* get_block_from_position(const Chunk* chunk, const glm::vec3& position)
{
	int x = (int)position.x;
	int y = (int)position.y;
	int z = (int)position.z;

	return &chunk->chunkBlocks[x][y][z];
}

void generate_chunk(Chunk* chunk)
{
	//for (int i = 0; i < CHUNK_VOLUME; i++)
	//{
	//	chunk->chunkBlocks.blocks
	//}
	//const glm::vec3 position = chunk->;
	//const glm::vec3 size = chunk->getSize();
	//const glm::vec4 color = chunk->getColor();
	//const GLuint blockTexID = chunk->getTIDfromBlockType();

	//for (int i = 0; i < 36; i++)
	//{
	//	const float* vertex = &BLOCK_VERTICES[i * 3];
	//	const float* normal = &BLOCK_NORMALS[i * 3];
	//	const float* uv = &BLOCK_UV[i * 2];
	//	m_VertexBuffer->vertex = glm::vec3(position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth);
	//	m_VertexBuffer->normal = glm::vec3(normal[0], normal[1], normal[2]);
	//	m_VertexBuffer->uv = glm::vec2(uv[0], uv[1]);
	//	m_VertexBuffer->tid = blockTexID;/*renderable->getTextureIDfromTypeID();*/
	//	m_VertexBuffer->color = c;
	//	m_VertexBuffer++;
	//}
}
