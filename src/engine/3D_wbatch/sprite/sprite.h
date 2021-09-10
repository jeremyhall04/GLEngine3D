#ifndef SPRITE_H
#define SPRITE_H

#include "../renderable2D.h"

namespace delta { namespace graphics {

	class Sprite : public Renderable2D
	{
	public:
		Sprite(float x, float y, float width, float height, const glm::vec4& color);
	};

}}

#endif // !SPRITE_H
