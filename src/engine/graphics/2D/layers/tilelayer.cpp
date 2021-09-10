#include "tilelayer.h"

namespace delta { namespace graphics {

	TileLayer::TileLayer(Shader* shader)
		: Layer(new Renderer2D(), shader, glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
	{
	}

	TileLayer::~TileLayer()
	{
	}

}}
