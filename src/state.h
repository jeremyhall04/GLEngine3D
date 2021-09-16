#ifndef STATE_H
#define STATE_H

#include "GLcommon.h"
#include "utils/camera.h"
#include "graphics/window/window.h"
#include "graphics/shader.h"
#include "graphics/3D/renderer3d.h"
#include "graphics/3D/blocks/block.h"

struct State
{
	Window* window;
	Renderer3D renderer;
	PerspectiveCamera* camera;
};

void init_state(Window* window, Shader* shader, Renderer3D* renderer, Block* blocks, PerspectiveCamera* camera);

// global state
extern struct State state;


#endif // !STATE_H
