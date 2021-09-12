#include "window.h"
#include "../../GLcommon.h"
#include "../../GLcommon_math.h"

namespace delta { namespace graphics {

	Window::Window()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "ERROR::GLFW::WINDOW CREATION FAILED\n" << std::endl;
			glfwTerminate();
			exit(1);
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		//glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "ERROR::GLFW::WINDOW CREATION FAILED\n" << std::endl;
			glfwTerminate();
			exit(1);
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSwapInterval(0);	// uncap fps

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "ERROR::GLAD::GLAD INITIALIZATION FAILED\n" << std::endl;
			exit(1);
		}
	}

	Window::~Window()
	{
		glfwTerminate();
		window = nullptr;
	}

	bool Window::isClose()
	{
		return glfwWindowShouldClose(window) == 1;
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

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void Window::processInput()
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
	}

	void Window::getMousePos(double& x, double& y)
	{
		glfwGetCursorPos(window, &x, &y);
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		//if (firstMouse)
		//{
		//	lastX = xpos;
		//	lastY = ypos;
		//	firstMouse = false;
		//}
	}

} }