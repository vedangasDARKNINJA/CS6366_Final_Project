#include "Mesh.hpp"

namespace RendererPBR
{
	Mesh::Mesh()
	{
		// Upload vertex data to OpenGL in child classes
	}

	Mesh::~Mesh()
	{
		delete m_IndexBuffer;
		delete m_VertexArray;
	}

	VertexArray* Mesh::GetVAO() const
	{
		return m_VertexArray;
	}

	IndexBuffer* Mesh::GetIBO() const
	{
		return m_IndexBuffer;
	}

	void Mesh::OnImGuiDraw()
	{
		transform.OnImGuiDraw(GetName());
	}
}