#ifndef WINDOW_H
#define WINDOW_H

#include "../../GLcommon.h"
#include "../../utils/camera.h"
#include "../../world/world.h"

const int SCR_WIDTH = 1400;
const int SCR_HEIGHT = 800;

extern PerspectiveCamera* g_CameraPtr; // external global
extern World* g_World;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
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
	void processInput(Player* player, float deltaTime);
	void getMousePos(double& x, double& y);

	GLFWwindow* context;
};

#endif

