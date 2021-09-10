#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "../GLcommon.h"
#include "../GLcommon_math.h"

namespace delta { namespace graphics {

	class Renderer
	{
	protected:
		std::vector<glm::mat4> m_TransformationStack;
		const glm::mat4* m_TransformationBack;

	protected:
		Renderer() 
		{
			m_TransformationStack.push_back(glm::mat4(1.0f));
			m_TransformationBack = &m_TransformationStack.back();
		};

	public:
		inline void push(glm::mat4 matrix, bool override = false)
		{
			if (override)
				m_TransformationStack.push_back(matrix);
			else
				m_TransformationStack.push_back(m_TransformationStack.back() * matrix);

			m_TransformationBack = &m_TransformationStack.back();
		}
		inline void pop()
		{
			// TODO: add to log!
			if (m_TransformationStack.size() > 1)
				m_TransformationStack.pop_back();

			m_TransformationBack = &m_TransformationStack.back();
		}

		virtual void begin() {}
		//virtual void submit() = 0;
		virtual void end() {}
		virtual void flush() = 0;
	};

} }

#endif // !RENDERER_H
