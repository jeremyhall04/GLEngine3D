#ifndef WINDOW_H
#define WINDOW_H

#include "../../GLcommon.h"
#include <iostream>

const int SCR_WIDTH = 960;
const int SCR_HEIGHT = 540;

//static bool firstMouse = true;
//static float lastX = SCR_WIDTH / 2.0f;
//static float lastY = SCR_HEIGHT / 2.0f;

namespace delta { namespace graphics {
	
	class Camera;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	class Window
	{
	public:
		Window();
		Window(const char* windowTitle, int width, int height);
		~Window();

		bool isClose();
		void clear();
		void update();
		void processInput();
		void getMousePos(double& x, double& y);

		GLFWwindow* context;
	};

} }
#endif

