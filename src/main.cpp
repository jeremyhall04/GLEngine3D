#include "Header.h"

#include "graphics/window/window.h"
#include "graphics/shader.h"

#include "blocks/block.h"
#include "graphics/renderable3D.h"
#include "graphics/renderer3d.h"
#include "graphics/texture/texture.h"

#include "utils/camera.h"
#include "utils/direction.h"
#include "utils/timer.h"

float orth_w = 16.0f * 2.0f, orth_h = 9.0f * 2.0f;

Timer time;
float timer = 0.0f;
UINT frames = 0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	Window window("3D Engine", SCR_WIDTH, SCR_HEIGHT);

	/*glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 ortho_ProjMatrix = glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -100.0f, 100.0f);
	glm::mat4 perspective_ProjMatrix = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.01f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	shader->setUniformMat4("pr_matrix", ortho_ProjMatrix);
	shader->setUniformMat4("pr_matrix", perspective_ProjMatrix);
	shader->setUniformMat4("vw_matrix", viewMatrix);*/
	g_CameraPtr = new PerspectiveCamera(0, 0, 10.0f, glm::vec3(0.0f, 0.0f, -1.0f), glm::radians(45.0f));

	//Shader* shader = new Shader("src/res/shaders/basic.vert", "src/res/shaders/basic.frag");
	Shader* shader = new Shader("res/shaders/texture.vert", "res/shaders/texture.frag");
	shader->enable();

	Texture texture("res/images/grass.png");
	texture.bind();

	Renderer3D renderer;

	const int chunkSize = 16;
	float blockSize = 1.0f;
	float blockX = 0.0f, blockY = 0.0f, blockZ = 0.0f;
	Block*** chunk;
	chunk = new Block * *[chunkSize];
	for (int i = 0; i < chunkSize; i++) {
		chunk[i] = new Block *[chunkSize];
		for (int j = 0; j < chunkSize; j++) {
			chunk[i][j] = new Block[chunkSize];
		}
	}

	for (int i = 0; i < chunkSize; i++) {
		blockX = i * blockSize;
		for (int j = 0; j < chunkSize; j++) {
			blockZ = j * blockSize;
			for (int k = 0; k < chunkSize; k++) {
				blockY = k * blockSize;
				Block* newCube = new Block(blockX, blockY, -blockZ, blockSize, glm::vec4(0, 1, 1, 1));
				chunk[i][j][k] = *newCube;
			}
		}
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
	shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	double x = 0.0, y = 0.0;
	while (!glfwWindowShouldClose(window.context))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();
		window.processInput();

		texture.bind();
		//shader->setUniform1i("textureSampler", texture.getID()-1);

		shader->enable();
		shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
		shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());
		//shader->setUniformMat4("ml_matrix", glm::rotate(glm::mat4(1.0f), glm::radians(time.elapsed() * 20.0f), glm::vec3(1.0f, 1.0f, 0.0f)));

		// Renderer
		renderer.begin();

		for (int i = 0; i < chunkSize; i++)
			for (int j = 0; j < chunkSize; j++)
				for (int k = 0; k < chunkSize; k++)
					renderer.submit(&chunk[i][j][k]);
		renderer.end();
		renderer.flush();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("\n%d fps", frames);
			frames = 0;
		}
	}

	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			delete[] chunk[i][j];
		}
		delete[] chunk[i];
	}
	delete[] chunk;

	return 0;
}