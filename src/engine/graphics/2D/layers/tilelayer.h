#ifndef TILE_LAYER_H
#define TILE_LAYER_H

#include "layer.h"
#include "../renderer2d.h"

namespace delta { namespace graphics {

	class TileLayer : public Layer
	{
	public:
		TileLayer(Shader* shader);
		~TileLayer();
	};

}}

#endif // !TILE_LAYER_H

