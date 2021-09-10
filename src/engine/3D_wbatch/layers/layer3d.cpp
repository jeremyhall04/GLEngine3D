#include "layer3d.h"

namespace delta { namespace graphics {

	Layer3D::Layer3D(Renderer3D* renderable, Shader* shader, glm::mat4 projectionMatrix)
		: m_Renderer(renderable), m_Shader(shader), m_ProjectionMatrix(projectionMatrix)
	{
		m_Shader->enable();
		m_Shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);
		m_Shader->disable();
	}

	Layer3D::~Layer3D()
	{
		delete m_Shader;
		delete m_Renderer;
		for (int i = 0; i < m_Renderables.size(); i++)
			delete m_Renderables[i];
	}

	void Layer3D::add(Renderable3D* renderable)
	{
		m_Renderables.push_back(renderable);
	}

	void Layer3D::render()
	{
		m_Shader->enable();

		m_Renderer->begin();

		for (const Renderable3D* renderable : m_Renderables)
			m_Renderer->submit(renderable);

		m_Renderer->end();

		m_Renderer->flush();
	}

}}