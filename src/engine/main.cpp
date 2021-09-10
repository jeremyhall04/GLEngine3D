#include "Header.h"

#include "graphics/window/window.h"
#include "graphics/shader.h"

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexbuffer.h"
#include "graphics/buffers/vertexarray.h"

#include "graphics/2D/renderer2d.h"
#include "graphics/2D/renderable2d.h"
#include "graphics/2D/sprite/sprite.h"
#include "graphics/2D/sprite/static_sprite.h"
#include "graphics/2D/layers/tilelayer.h"

#include "graphics/3D/renderable3D.h"
#include "graphics/3D/renderer3d.h"
#include "graphics/3D/blocks/block.h"
//#include "graphics/3D/renderable3d.h"
//#include "graphics/3D/renderer3d.h"
//#include "graphics/3D/block/block.h"
//#include "graphics/3D/layers/layer3d.h"
//#include "graphics/3D/layers/chunk.h"

#include "utils/timer.h"
#include "utils/camera.h"

#define RENDER_3D 1
#define RENDER_9K_SPRITES 1

using namespace delta;
using namespace graphics;
using namespace utils;

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

Timer time;
float timer = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
UINT frames = 0;

int main()
{
	Window window("3D Engine", SCR_WIDTH, SCR_HEIGHT);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 ortho_ProjMatrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -100.0f, 100.0f);
	glm::mat4 perspective_ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	/*std::cout << "\nviewMatrix = ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << "\n";
		for (int j = 0; j < 4; j++)
		{
			std::cout << viewMatrix[j][i] << "\t";
		}
	}
	std::cout << "\nprojectionMatrix = ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << "\n";
		for (int j = 0; j < 4; j++)
		{
			std::cout << perspective_ProjMatrix[j][i] << "\t";
		}
	}*/


	Shader* shader = new Shader("src/engine/graphics/shaders/basic.vert", "src/engine/graphics/shaders/basic.frag");
	shader->enable();

	OrthoCamera orthoCam(0, 0, 20.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(-16.0f, 16.0), glm::vec2(-9.0f, 9.0f));
	PerspectiveCamera perspectiveCam(0, 0, 10.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(45.0f));

	//shader->setUniformMat4("pr_matrix", orthoCam.getProjectionMatrix());
	shader->setUniformMat4("pr_matrix", perspectiveCam.getProjectionMatrix());
	//shader->setUniformMat4("vw_matrix", perspectiveCam.getViewMatrix());

	/*std::cout << "\nperspective viewMatrix = ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << "\n";
		for (int j = 0; j < 4; j++)
		{
			std::cout << perspectiveCam.getViewMatrix()[j][i] << "\t";
		}
	}
	std::cout << "\nperspective projectionMatrix = ";
	for (int i = 0; i < 4; i++)
	{
		std::cout << "\n";
		for (int j = 0; j < 4; j++)
		{
			std::cout << perspectiveCam.getProjectionMatrix()[j][i] << "\t";
		}
	}*/

	//shader->setUniformMat4("pr_matrix", ortho_ProjMatrix);
	//shader->setUniformMat4("pr_matrix", perspective_ProjMatrix);
	shader->setUniformMat4("vw_matrix", viewMatrix);


#if RENDER_3D

	Block block(0, 0, 0, 5, glm::vec4(1, 0, 1, 1));
	Renderer3D renderer;

#else

	TileLayer layer(shader), layer2(shader2);

#if RENDER_9K_SPRITES

	for (float y = -orth_h / 2.0f; y < orth_h / 2.0f; y += 0.25f)
	{
		for (float x = -orth_w / 2.0f; x < orth_w / 2.0f; x += 0.25f)
		{
			layer.add(new Sprite(x, y, 0.25f, 0.25f, glm::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
		}
	}

#else

	Sprite* button = new Sprite(0.5f, 0.5f, 5.0f, 2.0f, glm::vec4(1, 0, 1, 1));
	layer.add(button);
	layer.add(new Sprite(-15.0f, 6.0f, 6, 3, glm::vec4(1, 1, 1, 1)));

#endif

#endif

	double x = 0.0, y = 0.0;
	while (!glfwWindowShouldClose(window.window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//window.clear();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		window.processInput();
		window.getMousePos(x, y);

		//orthoCam.processCameraInput(window.window);
		//orthoCam.processMouseMovement(x, y, true);
		//orthoCam.update();

#if RENDER_3D
		//shader->enable();
		shader->setUniformMat4("ml_matrix", glm::rotate(glm::mat4(1.0f), glm::radians(time.elapsed() * 20.0f), glm::vec3(1.0f, 1.0f, 0.0f)));
		shader->setUniform2f("light_pos", glm::vec2(glm::vec2((float)(x * orth_w / SCR_WIDTH - (orth_w / 2.0f)), (float)(orth_h / 2.0f - y * orth_h / SCR_HEIGHT))));
		renderer.begin();

		renderer.submit(&block);

		renderer.end();
		renderer.flush();

#else
		shader->enable();
		shader->setUniform2f("light_pos", glm::vec2(glm::vec2((float)(x * orth_w / SCR_WIDTH - (orth_w / 2.0f)), (float)(orth_h / 2.0f - y * orth_h / SCR_HEIGHT))));
		layer.render();
#endif

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("\n%d fps", frames);
			frames = 0;
		}
	}

	return 0;
}