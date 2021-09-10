#ifndef LAYER_3D_H
#define LAYER_3D_H

#include "../../../GLcommon.h"
#include "../renderer3d.h"
#include "../block/block.h"
#include "../../shader.h"

namespace delta { namespace graphics {

	class Layer3D
	{
	protected:
		Renderer3D* m_Renderer;
		std::vector<Renderable3D*> m_Renderables;
		Shader* m_Shader;
		glm::mat4 m_ProjectionMatrix;

	protected:
		Layer3D(Renderer3D* renderable, Shader* shader, glm::mat4 projectionMatrix);

	public:
		virtual ~Layer3D();
		virtual void add(Renderable3D* renderable);
		virtual void render();
	};

}}

#endif // !LAYER_3D_H
