#include "renderer3d.h"

#include <iostream>

Renderer3D::Renderer3D()
{
	shader = new Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	shader->enable();
	init();
	//initTextures();
}

Renderer3D::~Renderer3D()
{
	delete[] m_TextureSlotsPtr;
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

	glEnableVertexAttribArray(SHADER_VERTEX_INDEX_3D);
	glEnableVertexAttribArray(SHADER_UV_INDEX_3D);
	glEnableVertexAttribArray(SHADER_TID_INDEX_3D);
	glEnableVertexAttribArray(SHADER_COLOR_INDEX_3D);
	glVertexAttribPointer(SHADER_VERTEX_INDEX_3D,	4, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)0);
	glVertexAttribPointer(SHADER_UV_INDEX_3D,		2, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::uv)));
	glVertexAttribPointer(SHADER_TID_INDEX_3D,		1, GL_FLOAT,		GL_FALSE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::tid)));
	glVertexAttribPointer(SHADER_COLOR_INDEX_3D,	4, GL_UNSIGNED_BYTE, GL_TRUE, VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData3D, VertexData3D::color)));

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

	m_IBO = new IndexBuffer(allocate108BlockIndices(), MAX_INDICES);
	glBindVertexArray(0);

	//generateTextureIndices();
	//for (int i = 0; i < MAX_TEXTURES; i++)
	//	m_TextureSlotsVec.push_back(0);		// set all texture slots to 0
	m_TextureSlotsPtr = new GLuint[MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; i++)
		m_TextureSlotsPtr[i] = 0;
	int samplers[MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; i++)
		samplers[i] = i;

	shader->setUniform1iv("textures", MAX_TEXTURES, samplers);
}

void Renderer3D::generateTextureIndices()
{
	for (int i = 0; i < MAX_TEXTURES; i++)
		m_TextureSlotsVec.push_back(i);

	Texture** m_Textures = new Texture * [MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; i++)
		m_Textures[i] = new Texture();

	Texture _default("res/images/default.png"), grass("res/images/grass.png"), stone("res/images/stone.png");

	m_Textures[0] = &_default;
	m_Textures[1] = &grass;
	m_Textures[2] = &stone;

	printf("\nreal  ids = %d, %d, %d", _default.getID(), grass.getID(), stone.getID());
	printf("\ntexture ids = %d, %d, %d", m_Textures[0]->getID(), m_Textures[1]->getID(), m_Textures[2]->getID());

	for (int i = 0; i < 3; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]->getID());
	}
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

//const glm::vec3* Renderer3D::checkFaceToRender()
//{
//
//}

void Renderer3D::begin()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBuffer = (VertexData3D*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void Renderer3D::submit(const Renderable3D* renderable)
{
	const glm::vec4 position = renderable->getPosition();
	const glm::vec3 size = renderable->getSize();
	const glm::vec4 color = renderable->getColor();

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
	//	if (m_TextureSlotsPtr[i] == (GLuint)renderable->getTextureIDfromTypeID())
	//	{
	//		textureIndex = (float)i;
	//		break;
	//	}
	//}

	//if (textureIndex == 0.0f)
	//{
	//	textureIndex = (float)m_TextureSlotIndex; 
	//	m_TextureSlotsPtr[m_TextureSlotIndex] = renderable->getTextureIDfromTypeID();
	//	m_TextureSlotIndex++;
	//}

	for (int i = 0; i < 36; i++)
	{
		const float* vertex = &BLOCK_VERTICES[i * 3];
		const float* uv = &BLOCK_UV[i * 2];
		m_VertexBuffer->vertex = glm::vec4(position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth, position.w);
		//printf("\nblock vertices = <%f, %f, %f>", position.x + vertex[0] * width, position.y + vertex[1] * height, position.z + vertex[2] * depth);
		//if (i == 5 || i == 11 || i == 17 || i == 23 || i == 29)
		//	printf("\n");
		m_VertexBuffer->uv = glm::vec2(uv[0], uv[1]);
		m_VertexBuffer->tid = renderable->getTextureIDfromTypeID();
		m_VertexBuffer->color = c;
		m_VertexBuffer++;
	}

	//std::vector<int>::iterator it = std::find(m_TextureIndices.begin(), m_TextureIndices.end(), renderable->getTID());
	//if (it != m_TextureIndices.end())
	//{
	//	// texture ID found, therefore has not yet been bound
	//	m_TextureIndices.erase(it);
	//}

	m_IndexCount += 36;
}

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
	m_IBO->bind();

	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

	m_IBO->unbind();
	glBindVertexArray(0);

	m_IndexCount = 0;
}