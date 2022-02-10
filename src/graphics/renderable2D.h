#ifndef RENDERABLE_2D
#define RENDERABLE_2D

#include "../GLcommon.h"
#include "../GLcommon_math.h"
#include "buffers/VAO.h"
#include "buffers/VBO.h"
#include "shader.h"
#include "texture/texture.h"
#include "shader.h"
#include "texture/texture.h"

class Camera;

class Quad2D
{
private:
	glm::vec3 pos;
	float width, height;
	Shader* shader;
	Texture* texture;
	//VAO vao;
	//VBO vbo;
	unsigned int vbo, vao, ebo;

public:
	Quad2D();
	Quad2D(const glm::vec3 pos, float width, float height, const char* vertexPath, const char* fragPath, const char* texturePath);
	Quad2D(const glm::vec3 pos, const char* vertexPath, const char* fragPath, const char* texturePath);
	~Quad2D();

private:
	void init();
public:
	void update();
	void render(Camera* camera);
};

#endif // !RENDERABLE_2D
