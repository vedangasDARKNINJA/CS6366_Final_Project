#include "VertexBuffer.hpp"
#include <glad/glad.h>

namespace RendererPBR
{
	VertexBuffer::VertexBuffer(const void* data, size_t size)
	{
		glGenBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
	}

	VertexBuffer::~VertexBuffer()
	{
		Unbind();
		glDeleteBuffers(1, &m_RendererID);
	}

	void VertexBuffer::Bind()const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void VertexBuffer::Unbind()const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}
