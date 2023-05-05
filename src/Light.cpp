#include "Light.hpp"

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace RendererPBR
{
	Light::Light()
		:Ambient(0.05f),Diffuse(0.5f),Specular(1.0f)
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

	unsigned int Light::GetRenderMode() const
	{
		return GL_TRIANGLE_STRIP;
	}

	void Light::OnImGuiDraw()
	{
		ImGui::ColorEdit3("Ambient", &Ambient[0]);
		ImGui::ColorEdit3("Diffuse", &Diffuse[0]);
		ImGui::ColorEdit3("Specular", &Specular[0]);
	}
}