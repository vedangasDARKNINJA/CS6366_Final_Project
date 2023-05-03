#pragma once

namespace RendererPBR
{
	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, size_t size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int m_RendererID;
	};
}