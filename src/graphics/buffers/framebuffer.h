#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "../../GLcommon.h"
#include "../../GLcommon_math.h"

struct FramebufferSpec
{
	GLuint Width, Height;
	//FramebufferFormat Format = 
	GLuint Samples = 1;

	bool SwapChainTarget = false;	// glBindFramebuffer(0); -> want to render to screen
};

class Framebuffer
{
private:
	GLuint m_ID;
	GLuint m_ColorAttachment;
	GLuint m_TextureAttachment;
	FramebufferSpec* specs;
public:
	Framebuffer();
	~Framebuffer();
	void create(const FramebufferSpec& spec);
	void Invalidate();
	const FramebufferSpec getSpec() const { return *specs; };
	//FramebufferSpec& getSpec() { return specs; };
};

#endif // !FRAME_BUFFER_H