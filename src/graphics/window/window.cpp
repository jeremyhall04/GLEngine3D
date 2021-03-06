#include "window.h"
#include <iostream>

#include "../../utils/crtdebug.h"
#include "../../entity/player.h"

extern PerspectiveCamera* g_CameraPtr = 0;
extern World* g_World = 0;
bool isWireframe = false;

Window::Window()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	context = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (context == NULL)
	{
		std::cout << "ERROR::GLFW::WINDOW CREATION FAILED\n" << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(context);
	glfwSetFramebufferSizeCallback(context, framebuffer_size_callback);
	//glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::GLAD INITIALIZATION FAILED\n" << std::endl;
		exit(1);
	}
}

Window::Window(const char* windowTitle, int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	context = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	if (context == NULL)
	{
		std::cout << "ERROR::GLFW::WINDOW CREATION FAILED\n" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwMakeContextCurrent(context);
	glfwSetFramebufferSizeCallback(context, framebuffer_size_callback);
	glfwSetCursorPosCallback(context, mouse_callback);
	glfwSetMouseButtonCallback(context, mouse_button_callback);
	glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(0);	// 0 for uncap fps

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "ERROR::GLAD::GLAD INITIALIZATION FAILED\n" << std::endl;
		exit(1);
	}
}

Window::~Window()
{
	glfwTerminate();
	context = nullptr;
#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif
}

bool Window::isClose()
{
	return glfwWindowShouldClose(context) == 1;
}

void Window::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update()
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "\nERROR::WINDOW::UPDATE::OpenGl error::" << error;

	g_CameraPtr->update();

	if (isWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glfwSwapBuffers(context);
	glfwPollEvents();
}

void Window::processInput(Player* player, float deltaTime)
{
	static bool wasPressed, lmb_pressed;
	if (glfwGetKey(context, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(context, true);

	// keyboard input
	if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
 		g_CameraPtr->processKeyboardInput(CameraMovement::RIGHT, deltaTime);
	if (glfwGetKey(context, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::UP, deltaTime);
	if (glfwGetKey(context, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::DOWN, deltaTime);



	// Player block inventory
	if (glfwGetKey(context, GLFW_KEY_1) == GLFW_PRESS)
		player->setActiveSlot(0);
	if (glfwGetKey(context, GLFW_KEY_2) == GLFW_PRESS)
		player->setActiveSlot(1);
	if (glfwGetKey(context, GLFW_KEY_3) == GLFW_PRESS)
		player->setActiveSlot(2);
	
	if (glfwGetKey(context, GLFW_KEY_O) == GLFW_PRESS && !wasPressed)
	{
		isWireframe = !isWireframe;
		wasPressed = true;
	}
	if (glfwGetKey(context, GLFW_KEY_O) == GLFW_RELEASE)
		wasPressed = false;

}

void Window::getMousePos(double& x, double& y)
{
	glfwGetCursorPos(context, &x, &y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	g_CameraPtr->updateAspect((float)width / (float)height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		m_MouseLastX = (float)SCR_WIDTH / 2.0f;
		m_MouseLastY = (float)SCR_HEIGHT / 2.0f;
		glfwSetCursorPos(window, m_MouseLastX, m_MouseLastY);
		m_FirstMouse = false;
	}

	float xOffset = (float)xpos - m_MouseLastX;
	float yOffset = m_MouseLastY - (float)ypos;

	m_MouseLastX = (float)xpos;
	m_MouseLastY = (float)ypos;
	xOffset *= m_MouseSensitivity;
	yOffset *= m_MouseSensitivity;

	g_CameraPtr->processMouseMovement(xOffset, yOffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	g_World->mouseEvent(button, action);
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::FORWARD, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::BACKWARD, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::LEFT, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::RIGHT, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::UP, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		g_CameraPtr->processKeyboardInput(CameraMovement::DOWN, 0.0f);
}