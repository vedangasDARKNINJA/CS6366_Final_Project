#include "Renderbuffer.hpp"
#include <iostream>
#include <glad/glad.h>

#include "Texture.hpp"

namespace RendererPBR
{
	Renderbuffer::Renderbuffer(unsigned int width, unsigned int height)
		:m_RendererID(0),m_Width(width),m_Height(height)
	{
		glGenRenderbuffers(1, &m_RendererID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID); 
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	Renderbuffer::~Renderbuffer()
	{
		glDeleteRenderbuffers(1, &m_RendererID);
	}

	void Renderbuffer::Bind()const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
	}

	void Renderbuffer::Unbind()const
	{
		glBindRenderbuffer(GL_FRAMEBUFFER, 0);
	}

	unsigned int Renderbuffer::GetID() const
	{
		return m_RendererID;
	}
}