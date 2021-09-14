#include "window.h"

#include <iostream>

extern delta::utils::PerspectiveCamera* g_CameraPtr = 0;

namespace delta { namespace graphics {

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
		//glfwSetKeyCallback(context, keyboard_callback);
		//glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR);
		glfwSetInputMode(context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSwapInterval(1);	// uncap fps

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

		glfwSwapBuffers(context);
		glfwPollEvents();
	}

	void Window::processInput()
	{
		if (glfwGetKey(context, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(context, true);

		if (glfwGetKey(context, GLFW_KEY_W) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::FORWARD, 0.0f);
		if (glfwGetKey(context, GLFW_KEY_S) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::BACKWARD, 0.0f);
		if (glfwGetKey(context, GLFW_KEY_A) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::LEFT, 0.0f);
		if (glfwGetKey(context, GLFW_KEY_D) == GLFW_PRESS)
 			g_CameraPtr->processKeyboardInput(utils::RIGHT, 0.0f);
		if (glfwGetKey(context, GLFW_KEY_SPACE) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::UP, 0.0f);
		if (glfwGetKey(context, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::DOWN, 0.0f);
	}

	void Window::getMousePos(double& x, double& y)
	{
		glfwGetCursorPos(context, &x, &y);
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
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

		float xOffset = xpos - m_MouseLastX;
		float yOffset = m_MouseLastY - ypos;

		m_MouseLastX = xpos;
		m_MouseLastY = ypos;
		xOffset *= m_MouseSensitivity;
		yOffset *= m_MouseSensitivity;

		g_CameraPtr->processMouseMovement(xOffset, yOffset);
	}

	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//if ((key == GLFW_KEY_W) && (action == GLFW_PRESS))
		//	g_CameraPtr->processKeyboardInput(utils::FORWARD, 0.0f);
		//if ((key == GLFW_KEY_S) && (action == GLFW_PRESS))
		//	g_CameraPtr->processKeyboardInput(utils::BACKWARD, 0.0f);
		//if ((key == GLFW_KEY_A) && (action == GLFW_PRESS))
		//	g_CameraPtr->processKeyboardInput(utils::LEFT, 0.0f);
		//if ((key == GLFW_KEY_D) && (action == GLFW_PRESS))
		//	g_CameraPtr->processKeyboardInput(utils::RIGHT, 0.0f);
		//if ((key == GLFW_KEY_SPACE) && (action == GLFW_PRESS))
		//	g_CameraPtr->processKeyboardInput(utils::UP, 0.0f);
		//if ((key == GLFW_KEY_LEFT_SHIFT) && (action == GLFW_PRESS))
		//	g_CameraPtr->processKeyboardInput(utils::DOWN, 0.0f);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::FORWARD, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::BACKWARD, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::LEFT, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::RIGHT, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::UP, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			g_CameraPtr->processKeyboardInput(utils::DOWN, 0.0f);

	}


} }