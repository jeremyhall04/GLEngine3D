#include "renderer3d.h"
#include "../blocks/chunk.h"
#include "../world/world.h"
#include <iostream>

Renderer3D::Renderer3D()
{
	shader = new Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	shader->enable();
	//shader->setUniform3f("lightPos", glm::vec3(0.0f, 0.0f, 2.0f)); // for lighting
	//shader->setUniform3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	init();
	generateTextures();
}

Renderer3D::~Renderer3D()
{
	for (int i = 0; i < MAX_TEXTURES; i++)
		delete m_Textures[i];
	delete[] m_TextureIndices;

	if (m_IBO != nullptr)
		delete m_IBO;

	glDeleteBuffers(1, &m_VBO);
}

void Renderer3D::init()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
	glEnableVertexAttribArray(SHADER_NORMAL_INDEX);
	glEnableVertexAttribArray(SHADER_UV_INDEX);
	glEnableVertexAttribArray(SHADER_TID_INDEX);
	glEnableVertexAttribArray(SHADER_COLOR_INDEX);
	glVertexAttribPointer(SHADER_VERTEX_INDEX,	3, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)0);
	glVertexAttribPointer(SHADER_NORMAL_INDEX,	3, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::normal)));
	glVertexAttribPointer(SHADER_UV_INDEX,		2, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::uv)));
	glVertexAttribPointer(SHADER_TID_INDEX,		1, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::tid)));
	glVertexAttribPointer(SHADER_COLOR_INDEX,	4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::color)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);	// unbind array buffer

	/*GLuint* indices = new GLuint[REN3D_MAX_INDICES];
	GLuint offset = 0;
	for (int i = 0; i < REN3D_MAX_INDICES; i += 36)
	{
		for (int j = 0; j < 36; j++)
			indices[i + j] = BLOCK_INDICES[j] + offset;
		offset += 8;
	}

	m_IBO = new IndexBuffer(indices, REN3D_MAX_INDICES);
	delete[] indices;*/

	//m_IBO = new IndexBuffer(allocate108BlockIndices(), MAX_INDICES);
	glBindVertexArray(0);
}

void Renderer3D::generateTextures()
{
	m_TextureIndices = new int[MAX_INDICES];

	m_Textures[0] = new Texture("res/images/default.png");
	m_Textures[1] = new Texture("res/images/dirt.png");
	m_Textures[2] = new Texture("res/images/grass.png");
	m_Textures[4] = new Texture("res/images/grass_side.png");
	m_Textures[3] = new Texture("res/images/stone.png");

	for (GLuint i = 0; i < MAX_TEXTURES; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]->getID());
		m_TextureIndices[i] = i;
	}

	shader->setUniform1iv("textures", (int)MAX_TEXTURES, m_TextureIndices);

	// Initialize texture array
	/*//const int nTexLayers = 3;
	//const char* paths[nTexLayers];
	//paths[0] = "res/images/default.png";
	//paths[1] = "res/images/grass.png";
	//paths[2] = "res/images/stone.png";
	//int width = _default.getWidth(), height = _default.getHeight(), channels;
	//unsigned char* data;

	//GLuint texID;
	//glGenTextures(1, &texID);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, texID);

	//glTexImage3D(GL_TEXTURE_2D_ARRAY,	// target
	//				0,			// level
	//				GL_RGB,		// internal format
	//				width, height, nTexLayers,	// width, height, depth
	//				0,			// border
	//				GL_RGB,		// format
	//				GL_UNSIGNED_BYTE, // type
	//				0);

	//stbi_set_flip_vertically_on_load(true);
	//for (int i = 0; i < nTexLayers; i++)
	//{
	//	data = stbi_load(paths[i], &width, &height, &channels, 0);
	//	if (data)
	//	{
	//		//glTexSubImage2D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	//		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
	//			0,			// level
	//			0, 0, i,	// x, y, z offsets
	//			width, height, 1,
	//			GL_RGB,
	//			GL_UNSIGNED_BYTE,
	//			data);

	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	}
	//	else
	//		printf("\nERROR::Image(%d) did not load", i);
	//	stbi_image_free(data);
	//}

	//shader->setUniform1i("texArray", 0);
	//shader->setUniform1i("layer", 0);*/
}

GLuint* Renderer3D::allocateBlockIndices()
{
	GLuint* indices = new GLuint[MAX_INDICES];
	GLuint offset = 0;
	for (int i = 0; i < MAX_INDICES; i += 36)
	{
		for (int j = 0; j < 36; j++)
			indices[i + j] = BLOCK_INDICES[j] + offset;
		offset += 8;
	}

	if (indices != nullptr)
		return indices;
	else
		std::cout << "\nERROR::BLOCK::INITIALIZATION::Indices is nullptr";
}

GLuint* Renderer3D::allocate108BlockIndices()
{
	GLuint* indices = new GLuint[MAX_INDICES];
	GLuint offset = 0;
	for (int i = 0; i < MAX_INDICES; i++)
		indices[i] = i;
	return indices;
}

void Renderer3D::begin()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBuffer = (VertexData3D*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void Renderer3D::submit(const Renderable3D* renderable)
{
	const glm::vec3 position = renderable->getPosition();
	const glm::vec3 size = renderable->getSize();
	const glm::vec4 color = renderable->getColor();
	const GLuint blockTexID = renderable->getTIDfromBlockType();

	float width = size.x;
	float height = size.y;
	float depth = size.z;

	int r = (int)color.x * 255;
	int g = (int)color.y * 255;
	int b = (int)color.z * 255;
	int a = (int)color.w * 255;

	GLuint c = a << 24 | b << 16 | g << 8 | r;

	//for (GLuint i = 0; i < 8; i++)
	//{
	//	const float* vertex = &BLOCK_INDEXED_VERTICES[i * 3];
	//	const float* uv = &BLOCK_UV[i * 2];
	//	m_VertexBuffer->vertex = glm::vec4(position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth, position.w);
	//	m_VertexBuffer->uv = glm::vec2(uv[0], uv[1]);
	//	m_VertexBuffer->color = c;
	//	m_VertexBuffer++;
	//}
	//m_IndexCount += 36;

	float textureIndex = 0.0f;

	//for (int i = 0; i < m_TextureSlotIndex; i++)
	//{
	//	if (m_TextureSlots[i] == blockTexID)
	//	{
	//		textureIndex = (float)i;
	//		break;
	//	}
	//}

	//if (textureIndex == 0.0f)
	//{
	//	textureIndex = (float)m_TextureSlotIndex; 
	//	m_TextureSlots[m_TextureSlotIndex] = blockTexID;
	//	m_TextureSlotIndex++;
	//}

	int faceCount = 0;
	for (int i = 0; i < 36; i++)
	{
		if ((i == 6) || (i == 12) || (i == 18) || (i == 24) || (i == 30) || (i == 36))
			faceCount++;
		if (renderable->renderFace[faceCount] == false)
			continue;
		const float* vertex = &BLOCK_VERTICES[i * 3];
		const float* normal = &BLOCK_NORMALS[i * 3];
		const float* uv = &BLOCK_UV[i * 2];
		m_VertexBuffer->vertex = glm::vec3(position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth);
		m_VertexBuffer->normal = glm::vec3(normal[0], normal[1], normal[2]);
		m_VertexBuffer->uv = glm::vec3(uv[0], uv[1], blockTexID);
		m_VertexBuffer->tid = blockTexID;
		m_VertexBuffer->color = c;
		m_VertexBuffer++;
		m_IndexCount++;
	}

	//std::vector<int>::iterator it = std::find(m_TextureIndices.begin(), m_TextureIndices.end(), renderable->getTID());
	//if (it != m_TextureIndices.end())
	//{
	//	// texture ID found, therefore has not yet been bound
	//	m_TextureIndices.erase(it);
	//}

	//m_IndexCount += 36;
}

void Renderer3D::submitChunk(Chunk* chunk)
{
	//frustumCull(chunk);

	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_SIZE; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
			{
				//occlusionCull(chunk, glm::vec3(i, j, k));
				//if (chunk->chunkBlocks[i][j][k].isActive())
					//submit(&chunk->chunkBlocks[i][j][k]);
				if (chunk->get_block_from_pos_in_chunk(i, j, k)->isActive)
					submit(chunk->get_block_from_pos_in_chunk(i, j, k));
			}
}

void Renderer3D::submitChunkData(Chunk* chunks)
{
	for (int i = 0; i < CHUNK_SIZE; i++)
		for (int j = 0; j < CHUNK_SIZE; j++)
			for (int k = 0; k < CHUNK_SIZE; k++)
				submit(chunks->data[i + j * CHUNK_SIZE + k * CHUNK_SIZE_SQUARED]);
}

void Renderer3D::submitScene(World* world)
{
	for (int i = 0; i < WORLD_WIDTH; i++)
	{
		for (int j = 0; j < WORLD_HEIGHT; j++)
		{
			for (int k = 0; k < WORLD_DEPTH; k++)
			{
				submitChunk(world->chunks[i][j][k]);
			}
		}
	}
}

//void Renderer3D::submit(const Block* block)
//{
//	const glm::vec3 position = block->getPosition();
//	const glm::vec3 size = block->getSize();
//	const glm::vec4 color = block->getColor();
//	const GLuint blockTexID = block->getTIDfromBlockType();
//
//	float width = size.x;
//	float height = size.y;
//	float depth = size.z;
//
//	int r = (int)color.x * 255;
//	int g = (int)color.y * 255;
//	int b = (int)color.z * 255;
//	int a = (int)color.w * 255;
//
//	GLuint c = a << 24 | b << 16 | g << 8 | r;
//
//	for (int i = 0; i < 36; i++)
//	{
//		const float* vertex = &BLOCK_VERTICES[i * 3];
//		const float* normal = &BLOCK_NORMALS[i * 3];
//		const float* uv = &BLOCK_UV[i * 2];
//		m_VertexBuffer->vertex = glm::vec3(position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth);
//		m_VertexBuffer->normal = glm::vec3(normal[0], normal[1], normal[2]);
//		m_VertexBuffer->uv = glm::vec2(uv[0], uv[1]);
//		m_VertexBuffer->tid = blockTexID;/*renderable->getTextureIDfromTypeID();*/
//		m_VertexBuffer->color = c;
//		m_VertexBuffer++;
//	}
//
//	m_IndexCount += 36;
//}

void Renderer3D::end()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // unbind
}

void Renderer3D::flush()
{
	/*glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBuffer = (VertexData3D*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);


	for (int i = 0; i < 36; i++)
	{
		printf("\nvertex[%d] = \nvertices = %f, %f, %f\nuv = %f, %f", i, m_VertexBuffer->vertex.x, m_VertexBuffer->vertex.y, m_VertexBuffer->vertex.z, m_VertexBuffer->uv.x, m_VertexBuffer->uv.y);
		m_VertexBuffer++;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);*/

	glBindVertexArray(m_VAO);
	//m_IBO->bind();

	//glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);
	glDrawArrays(GL_TRIANGLES, 0, m_IndexCount);

	//m_IBO->unbind();
	glBindVertexArray(0);

	m_IndexCount = 0;
}

void generateTextures(Shader* shader)
{
	Texture* textures[MAX_TEXTURES];
	int* texIndices = new int[MAX_INDICES];

	textures[0] = new Texture("res/images/default.png");
	textures[1] = new Texture("res/images/grass.png");
	textures[2] = new Texture("res/images/stone.png");

	for (GLuint i = 0; i < MAX_TEXTURES; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->getID());
		texIndices[i] = i;
	}

	shader->setUniform1iv("textures", (int)MAX_TEXTURES, texIndices);

	for (int i = 0; i < MAX_TEXTURES; i++)
		delete textures[i];
	delete[] texIndices;
}
