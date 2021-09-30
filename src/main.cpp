#include "Header.h"

#include "graphics/window/window.h"
#include "graphics/shader.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

//#include "blocks/block.h"
#include "blocks/chunk.h"
#include "graphics/renderable3D.h"
#include "graphics/renderer3d.h"
#include "graphics/texture/texture.h"
#include "graphics/frustum/frustum.h"
#include "world/world.h"

#include "utils/camera.h"
#include "utils/direction.h"
#include "utils/timer.h"


float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

Timer time;
float timer = 0.0f;
UINT frames = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define RENDER3D 0

int main()
{
	Window window("3D Engine", SCR_WIDTH, SCR_HEIGHT);

	/*glm::vec3 camerapos = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 cameratarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameradirection = glm::normalize(camerapos - cameratarget);
	//glm::vec3 cameradirection = glm::normalize(cameratarget - camerapos);
	glm::vec3 worldup = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraright = glm::normalize(glm::cross(worldup, cameradirection));
	glm::vec3 cameraup = glm::cross(cameradirection, cameraright);

	glm::mat4 ortho_projmatrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -100.0f, 100.0f);
	glm::mat4 perspective_projmatrix = glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.01f, 100.0f);

	glm::mat4 viewmatrix = glm::lookAt(camerapos, cameratarget, cameraup);
	glm::mat4 viewmatrix = glm::lookAt(camerapos, camerapos + cameradirection, cameraup);

	shader->setuniformmat4("pr_matrix", ortho_projmatrix);
	shader->setuniformmat4("pr_matrix", perspective_projmatrix);
	shader->setuniformmat4("vw_matrix", viewmatrix);*/

	int i = 6;

	g_CameraPtr = new PerspectiveCamera(6.0f, 2.0f, 9.0f, glm::vec3(0.0f, 0.0f, -1.0f), glm::radians(45.0f));	// external in window.h
	glm::vec3 ray = g_CameraPtr->getDirection();

	World world;
	Renderer3D renderer;
	Shader* shader = renderer.shader;
	shader->enable();

	Frustum frustum(g_CameraPtr);

	/*Shader* shader2D = new Shader("res/shaders/quad.vert", "res/shaders/quad.frag");

	float quadVertices[] = {
		// position		// uv
		 0.5f,  0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 1.0f, 1.0f,
	};

	shader2D->enable();
	GLuint quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	Texture quadTex("res/images/grass.png");
	//shader2D->setUniform1i("quadTexture", quadTex.getID());*/

	//world.chunks[1][0][1]->isEmpty = true;
	//Chunk* c = world.chunks[1][0][0];

	//for (int j = 0; j < CHUNK_SIZE; j++)
	//{
	//	for (int i = 0; i < CHUNK_SIZE; i++)
	//	{
	//		c->getBlockFromIndex(i, j, CHUNK_SIZE - 1)->renderFace[1] = false;
	//	}
	//}

	glEnable(GL_DEPTH_TEST);
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);*/

	shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
	shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());

	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	double x = 0.0, y = 0.0;
	while (!glfwWindowShouldClose(window.context))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();
		window.processInput(deltaTime);
		
		//ray = g_CameraPtr->getDirection();
		//world.update();

		shader->enable();
		//shader->setUniform3f("viewPos", g_CameraPtr->getPosition());	// for specular lighting
		shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
		shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());
		//shader->setUniformMat4("ml_matrix", glm::rotate(glm::mat4(1.0f), glm::radians(time.elapsed() * 20.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	
		// Renderer
		renderer.begin();

		renderer.submitScene(&world);

		renderer.end();
		renderer.flush();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			//printf("\nrendering %d of %d blocks", count, nBlocks);
			timer += 1.0f;
			printf("\n%d fps", frames);
			frames = 0;
		}
	}

	delete g_CameraPtr;

	return 0;
}