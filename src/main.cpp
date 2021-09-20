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
	shader->enable();

	/*const int chunkSize = 16;
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
	}*/

	Block* block = new Block(0, 0, 0, 1, BlockType::Grass);
	Block* block1 = new Block(-1, 0, 0, 1, BlockType::Stone);
	Block* block2 = new Block(-1, 1, 0, 1, BlockType::Stone);
	Block* block3 = new Block(1, 0, 0, 1, BlockType::Grass);

	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	//shader->setUniformMat4("pr_matrix", perspective_projmatrix);
	//shader->setUniformMat4("vw_matrix", viewmatrix);
	shader->setUniformMat4("pr_matrix", g_CameraPtr->getProjectionMatrix());
	shader->setUniformMat4("vw_matrix", g_CameraPtr->getViewMatrix());

	// Initialize texture array
	Texture _default("res/images/default.png"), grass("res/images/grass.png"), stone("res/images/stone.png");
	
	const int nTexLayers = 3;
	const char* paths[nTexLayers];
	paths[0] = "res/images/default.png";
	paths[1] = "res/images/grass.png";
	paths[2] = "res/images/stone.png";
	int width = _default.getWidth(), height = _default.getHeight(), channels;
	unsigned char* data;

	//GLuint texID;
	//glGenTextures(1, &texID);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D_ARRAY, texID);

	//glTexImage3D(GL_TEXTURE_2D_ARRAY,	// target
	//				0,			// level
	//				GL_RGB,		// internal format
	//				width, height, nTexLayers,	// width, height, depth
	//				0,			// border
	//				GL_RGB,		// format
	//				GL_UNSIGNED_BYTE, // type
	//				0);

	//stbi_set_flip_vertically_on_load(true);
	//for (int i = 0; i < nTexLayers; i++)
	//{
	//	data = stbi_load(paths[i], &width, &height, &channels, 0);
	//	if (data)
	//	{
	//		//glTexSubImage2D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	//		glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
	//			0,			// level
	//			0, 0, i,	// x, y, z offsets
	//			width, height, 1,
	//			GL_RGB,
	//			GL_UNSIGNED_BYTE,
	//			data);

	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//	}
	//	else
	//		printf("\nERROR::Image(%d) did not load", i);
	//	stbi_image_free(data);
	//}

	//shader->setUniform1i("texArray", 0);
	//shader->setUniform1i("layer", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, (GLuint)_default.getID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, (GLuint)grass.getID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, (GLuint)stone.getID());

	int texIndices[] = { 0, 1, 2 };
	int nIndices = sizeof(texIndices) / sizeof(int);
	shader->setUniform1iv("textures", nIndices, texIndices);

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

		//for (int i = 0; i < chunkSize; i++)
		//	for (int j = 0; j < chunkSize; j++)
		//		for (int k = 0; k < chunkSize; k++)
		//			renderer.submit(&chunk[i][j][k]);
		renderer.submit(block);
		renderer.submit(block1);
		renderer.submit(block2);
		renderer.submit(block3);

		renderer.end();
		renderer.flush();

		window.update();

		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("\n%d fps", frames);
			index + 1 >= 3 ? 0 : index++;
			shader->setUniform1i("layer", index);
			frames = 0;
		}
	}

	//for (int i = 0; i < chunkSize; i++) {
	//	for (int j = 0; j < chunkSize; j++) {
	//		delete[] chunk[i][j];
	//	}
	//	delete[] chunk[i];
	//}
	//delete[] chunk;

	return 0;
}