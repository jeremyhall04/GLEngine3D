#ifndef WINDOW_H
#define WINDOW_H

#include "../../GLcommon.h"
#include "../../utils/camera.h"

const int SCR_WIDTH = 960;
const int SCR_HEIGHT = 540;

const float m_CameraSpeed = 0.1f;
const float m_MouseSensitivity = 0.1f;
static bool m_FirstMouse = true;
static float m_MouseLastX, m_MouseLastY;

extern delta::utils::PerspectiveCamera* g_CameraPtr;

namespace delta { namespace graphics {
	
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

