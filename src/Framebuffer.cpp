#include "Framebuffer.hpp"
#include <iostream>
#include <glad/glad.h>

#include "Texture.hpp"
#include "Renderbuffer.hpp"

namespace RendererPBR
{
	Framebuffer::Framebuffer()
		:m_RendererID(0)
	{
		glGenFramebuffers(1, &m_RendererID);
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	bool Framebuffer::IsComplete() const
	{
		Bind();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "[ERROR]: Framebuffer is not complete." << std::endl;
			Unbind();
			return false;
		}
		
		Unbind();
		return true;
	}

	void Framebuffer::Bind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void Framebuffer::Unbind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::AttachTexture(Texture* texture)
	{
		if (!texture)
		{
			return;
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetTextureID(), 0);
	}

	void Framebuffer::AttachRenderBuffer(Renderbuffer* renderbuffer)
	{
		if (!renderbuffer)
		{
			return;
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->GetID());
	}

	unsigned int Framebuffer::GetID() const
	{
		return m_RendererID;
	}
}