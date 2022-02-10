#ifndef RENDERER_3D_H
#define RENDERER_3D_H

#include <cstddef>
#include <vector>
#include "../GLcommon.h"
#include "../GLcommon_math.h"
#include "frustum/frustum.h"
#include "renderable3D.h"
#include "texture/texture.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"
#include "skybox.h"
#include "shader.h"

#define MAX_BLOCKS		50000 
#define VERTEX_SIZE		sizeof(VertexData3D)				// 20 bytes
#define BLOCK_SIZE		VERTEX_SIZE * 108
#define BUFFER_SIZE		BLOCK_SIZE * MAX_BLOCKS
#define MAX_INDICES		MAX_BLOCKS * 108
#define MAX_TEXTURES	6

#define SHADER_VERTEX_INDEX	0
#define SHADER_NORMAL_INDEX	1
#define SHADER_UV_INDEX		2
#define SHADER_TID_INDEX	3
#define SHADER_COLOR_INDEX	4

class Block;
class Chunk;
class ChunkMesh;
class World;
class PerspectiveCamera;

class Renderer3D
{
private:
	GLuint m_VAO;
	GLuint m_VBO;
	VAO vao;
	VBO vbo;
	GLsizei m_IndexCount;
	IndexBuffer* m_IBO;
	VertexData3D* m_VertexBuffer;

	Texture* m_Textures[MAX_TEXTURES];
	int* m_TextureIndices;

	Skybox m_Skybox;
	Texture* m_SkyboxTexture;
public:
	Shader* shader;
	Shader* skyboxShader;

public:
	Renderer3D();
	~Renderer3D();

private:
	void initialize();
	void initBuffers();
	GLuint* allocateBlockIndices();
	GLuint* allocate108BlockIndices();

public:
	void generateTextures();
	VertexData3D* getVertexBuffer() { return m_VertexBuffer; };
	void begin();
	void submitCube(const Renderable3D* renderable);
	void submitScene(World* world);
	void submitChunk(Chunk* chunk);
	void end();
	void flush();

	void renderWorld(World* world);
	void submitChunkMesh(ChunkMesh* mesh);

	void renderSkybox();
};

void generateTextures(Shader* shader);

#endif // !RENDERER_3D_H

