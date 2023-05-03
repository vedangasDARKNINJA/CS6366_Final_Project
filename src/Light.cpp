#include "Light.hpp"

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

namespace RendererPBR
{
	Light::Light()
		:m_LightColor(1.0f)
	{
		static const float vertices[] =
		{
			// Position			// Normals		   //Texure Coordinates
		   -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top right
			0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // top left 
			0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // bottom left
			0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
		};

		static const unsigned int indices[] =
		{
			1, 0, 2, 3
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

	void Light::SetColor(glm::vec3 lightColor)
	{
		m_LightColor = lightColor;
	}

	glm::vec4 Light::GetColor() const
	{
		return glm::vec4(m_LightColor,1.0f);
	}

	unsigned int Light::GetRenderMode() const
	{
		return GL_TRIANGLE_STRIP;
	}
}