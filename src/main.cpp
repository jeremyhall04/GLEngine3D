#include <iostream>

#include "graphics/window/window.h"
#include "graphics/shader.h"

#include "blocks/chunk.h"
#include "graphics/renderable3D.h"
#include "graphics/renderer3d.h"
#include "graphics/texture/texture.h"
#include "world/world.h"
#include "entity/player.h"

#include "utils/camera.h"
#include "utils/timer.h"
#include "utils/crtdebug.h"

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

Timer time;
float timer = 0.0f;
UINT frames = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	Window window("3D Engine", SCR_WIDTH, SCR_HEIGHT);

	g_CameraPtr = new PerspectiveCamera(0.0f, 2.0f + 32.0f, 0.0f, glm::vec3(1.0f, 0.0f, 1.0f), glm::radians(45.0f));	// external in window.h

	Player player(g_CameraPtr);
	g_World = new World(&player);
	Renderer3D renderer;
	Shader* shader = renderer.shader;
	shader->enable();

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

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
	shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());

	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	double x = 0.0, y = 0.0;
	while (!glfwWindowShouldClose(window.context))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();
		window.processInput(deltaTime);
		
		player.update(g_CameraPtr);
		g_World->update();

		shader->enable();
		//shader->setUniform3f("viewPos", g_CameraPtr->getPosition());	// for specular lighting
		shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
		shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());
		//shader->setUniformMat4("ml_matrix", glm::rotate(glm::mat4(1.0f), glm::radians(time.elapsed() * 20.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
	
		// Renderer
		// CHUNKS
		renderer.begin();

		renderer.submitScene(g_World);

		renderer.end();
		renderer.flush();

		// SKYBOX
		renderer.skyboxShader->enable();
		renderer.skyboxShader->setUniformMat4("projection", g_CameraPtr->getProjectionMatrix());
		renderer.skyboxShader->setUniformMat4("view", glm::mat4(glm::mat3(g_CameraPtr->getViewMatrix())));
		renderer.renderSkybox();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("\n%d fps", frames);
			frames = 0;
		}
	}

	delete g_CameraPtr;
	delete g_World;

#if defined _DELETECOUNT
	printf("\nnewCount = %d", newCount);
#endif

	return 0;
}