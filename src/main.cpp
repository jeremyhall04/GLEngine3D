#include <iostream>

#include "graphics/window/window.h"
#include "graphics/shader.h"

#include "blocks/chunk.h"
#include "blocks/chunkmesh.h"
#include "graphics/renderable3D.h"
#include "graphics/renderer3d.h"
#include "graphics/renderable2D.h"
#include "graphics/texture/texture.h"
//#include "graphics/UI/crosshair.h"
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

// TODO: Implement hash table for chunk system, using chunkIDs.
// TODO: figure out how to make indexed cubes to reduce block size

int main()
{
	Window window("3D Engine", SCR_WIDTH, SCR_HEIGHT);

	g_CameraPtr = new PerspectiveCamera(glm::vec3(0.0f, 4.0f + 32.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f), glm::radians(45.0f));	// external in window.h
	OrthoCamera* camera2D = new OrthoCamera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1, 0, 1), glm::vec2(0.0f, 10), glm::vec2(0.0f, 10));

	Player player(g_CameraPtr);
	g_World = new World(&player);

	Renderer3D renderer;
	Shader* shader = renderer.shader;

	Quad2D* crosshair = new Quad2D(glm::vec3(4.5, 4.5, 0), 0.5f, 0.5f, "res/shaders/ui.vert", "res/shaders/ui.frag", "res/images/crosshair.png");
	Quad2D* inventorySlot = new Quad2D(glm::vec3(0, 9, 0), "res/shaders/ui.vert", "res/shaders/ui.frag", "res/images/inventory_slot_selected.png");
	Quad2D* inventorySlot2 = new Quad2D(glm::vec3(1, 9, 0), "res/shaders/ui.vert", "res/shaders/ui.frag", "res/images/inventory_slot.png");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	shader->enable();
	shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
	shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());

	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

	double x = 0.0, y = 0.0;
	InventoryManager* IM;
	IM = player.getInventory();
	while (!glfwWindowShouldClose(window.context))
	{
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();
		window.processInput(&player, deltaTime);
		
		player.update(g_CameraPtr);
		g_World->update(&player);

		shader->enable();
		shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
		shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());
	
		// Renderer
		// CHUNKS
		//renderer.begin();
		//renderer.submitScene(g_World);
		//renderer.end();
		//renderer.flush();


		// CHUNK MESH
		renderer.renderWorld(g_World);


		// SKYBOX
		renderer.skyboxShader->enable();
		renderer.skyboxShader->setUniformMat4("projection", g_CameraPtr->getProjectionMatrix());
		renderer.skyboxShader->setUniformMat4("view", glm::mat4(glm::mat3(g_CameraPtr->getViewMatrix())));
		renderer.renderSkybox();


		// RENDER UI
		crosshair->render(camera2D);
		inventorySlot->render(camera2D);
		inventorySlot2->render(camera2D);
		//shaderUI->enable();
		//shaderUI->setUniformMat4("projection", camera2D->getProjectionMatrix());
		//shaderUI->setUniformMat4("view", camera2D->getViewMatrix());
		//glBindVertexArray(vao);
		//textureUI->bind();
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("\n%d fps", frames);
			printf("\nSlot %d has %d blocks", IM->currSlot, IM->slot[IM->currSlot].stackSize);
			frames = 0;
		}
	}

	delete g_CameraPtr;
	delete g_World;

	delete camera2D;
	delete crosshair;
	delete inventorySlot;
	delete inventorySlot2;

#if defined _DELETECOUNT
	printf("\nnewCount = %d", newCount);
#endif

	return 0;
}