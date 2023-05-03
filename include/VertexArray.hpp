#pragma once
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

namespace RendererPBR
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererID = 0;
	};
}