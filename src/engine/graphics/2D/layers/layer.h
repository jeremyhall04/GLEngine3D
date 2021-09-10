#ifndef LAYER_H
#define LAYER_H

#include "../renderer2d.h"
#include "../renderable2D.h"

namespace delta { namespace graphics {
	
	class Layer
	{
	protected:
		Renderer2D* m_Renderer;
		std::vector<Renderable2D*> m_Renderables;
		Shader* m_Shader;
		glm::mat4 m_ProjectionMatrix;

	protected:
		Layer(Renderer2D* renderable, Shader* shader, glm::mat4 projectionMatrix);

	public:
		virtual ~Layer();
		virtual void add(Renderable2D* renderable);
		virtual void render();
	};

}}

	#endif // !LAYER_H