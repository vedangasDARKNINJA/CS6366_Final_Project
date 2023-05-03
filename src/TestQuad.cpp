#include "TestQuad.hpp"

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"


namespace RendererPBR
{

	TestQuad::TestQuad()
	{
		static const float vertices[] =
		{
			// Position			// Normals		   //Texure Coordinates
			-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // top left 
			 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top right
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,// bottom left
			 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,// bottom right
		};

		static const unsigned int indices[] =
		{
			0, 2, 1, 3
		};

		m_VertexArray = new VertexArray();
		m_VertexArray->Bind();
		VertexBuffer vb(vertices, 32);
		m_IndexBuffer = new IndexBuffer(indices, 4);

		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(3); // Normals
		layout.Push<float>(2); // Tex Coordinates
		m_VertexArray->AddBuffer(vb, layout);

		// Unbind
		m_VertexArray->Unbind();
		m_IndexBuffer->Unbind();
	}
	unsigned int TestQuad::GetRenderMode() const
	{
		return GL_TRIANGLE_STRIP;
	}
}
