#include "renderable2D.h"
#include "../utils/camera.h"
#include "window/window.h"

Quad2D::Quad2D()
{
}

Quad2D::Quad2D(const glm::vec3 pos, float width, float height, const char* vertexPath, const char* fragPath, const char* texturePath)
	: pos(pos), width(width), height(height)
{
	shader = new Shader(vertexPath, fragPath);
	texture = new Texture(texturePath);

	init();
}

Quad2D::Quad2D(const glm::vec3 pos, const char* vertexPath, const char* fragPath, const char* texturePath)
	: pos(pos)
{
	shader = new Shader(vertexPath, fragPath);
	texture = new Texture(texturePath);

	width = 1;
	height = 1;

	init();
}

Quad2D::~Quad2D()
{
}

void Quad2D::init()
{
	// generate vertices for points
	float vertices[] = {
	 pos.x + width,		pos.y + height, 0.0f, 1.0f, 1.0f,
	 pos.x + width,		pos.y,			0.0f, 1.0f, 0.0f,
	 pos.x,				pos.y,			0.0f, 0.0f, 0.0f,
	 pos.x,				pos.y + height, 0.0f, 0.0f, 1.0f
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader->setUniform1i("tex", 0);
}

void Quad2D::update()
{
}

void Quad2D::render(Camera* camera)
{
	shader->enable();
	shader->setUniformMat4("projection", camera->getProjectionMatrix());
	shader->setUniformMat4("view", camera->getViewMatrix());
	glBindVertexArray(vao);
	texture->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
