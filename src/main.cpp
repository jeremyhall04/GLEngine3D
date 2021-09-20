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

Block*** createCubicChunk(float blockX, float blockY, float blockZ, float blockSize, const int& chunkSize);
void deleteCubicChunk(Block*** chunk, int chunkSize);
void submitCubeChunk(Renderer3D* renderer, Block*** chunk, int chunkSize);

int main()
{
	Window window("3D Engine", SCR_WIDTH, SCR_HEIGHT);

	/*glm::vec3 camerapos = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 cameratarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameradirection = glm::normalize(camerapos - cameratarget);
	glm::vec3 cameradirection = glm::normalize(cameratarget - camerapos);
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

	g_CameraPtr = new PerspectiveCamera(0, 0, 10.0f, glm::vec3(0.0f, 0.0f, -1.0f), glm::radians(45.0f));

	Renderer3D renderer;
	Shader* shader = renderer.shader;

	Block* block = new Block(-3, 0, 0, 1, BlockType::Grass);
	Block* block1 = new Block(-4, 0, 0, 1, BlockType::_Default);
	Block* block2 = new Block(-2, 1, 0, 1, BlockType::Stone);
	Block* block3 = new Block(2, 3, 2, 1, BlockType::Grass);

	int cubeChunkSize = 5;
	Block*** cubeChunk = createCubicChunk(0, 0, 0, 1.0f, cubeChunkSize);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//shader->setUniformMat4("pr_matrix", perspective_projmatrix);
	//shader->setUniformMat4("vw_matrix", viewmatrix);
	shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
	shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());

	//------------------------

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	double x = 0.0, y = 0.0;
	int index = 0;
	while (!glfwWindowShouldClose(window.context))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		window.clear();
		window.processInput();

		shader->enable();
		shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
		shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());
		//shader->setUniformMat4("ml_matrix", glm::rotate(glm::mat4(1.0f), glm::radians(time.elapsed() * 20.0f), glm::vec3(1.0f, 1.0f, 0.0f)));

		// Renderer
		renderer.begin();

		renderer.submit(block);	// grass
		renderer.submit(block1);	// default
		renderer.submit(block2);
		renderer.submit(block3);

		submitCubeChunk(&renderer, cubeChunk, cubeChunkSize);

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

	deleteCubicChunk(cubeChunk, cubeChunkSize);

	return 0;
}

Block*** createCubicChunk(float blockX, float blockY, float blockZ, float blockSize, const int& chunkSize)
{
	float bX = blockX, bY = blockY, bZ = blockZ;
	Block*** chunk;
	chunk = new Block * *[chunkSize];
	for (int i = 0; i < chunkSize; i++) {
		chunk[i] = new Block *[chunkSize];
		for (int j = 0; j < chunkSize; j++) {
			chunk[i][j] = new Block[chunkSize];
		}
	}

	for (int i = 0; i < chunkSize; i++) {
		bX = i * blockSize;
		for (int j = 0; j < chunkSize; j++) {
			bZ = j * blockSize;
			for (int k = 0; k < chunkSize; k++) {
				bY = k * blockSize;
				Block* newCube = new Block(bX, bY, -bZ, blockSize, BlockType::Stone);
				chunk[i][j][k] = *newCube;
			}
		}
	}
	return chunk;
}

void submitCubeChunk(Renderer3D* renderer, Block*** chunk, int chunkSize)
{
	for (int i = 0; i < chunkSize; i++)
		for (int j = 0; j < chunkSize; j++)
			for (int k = 0; k < chunkSize; k++)
				renderer->submit(&chunk[i][j][k]);
}

void deleteCubicChunk(Block*** chunk, int chunkSize)
{
	for (int i = 0; i < chunkSize; i++) {
		for (int j = 0; j < chunkSize; j++) {
			delete[] chunk[i][j];
		}
		delete[] chunk[i];
	}
	delete[] chunk;
}