#include "Renderer.hpp"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RendererPBR.hpp"
#include "Window.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Framebuffer.hpp"

namespace RendererPBR
{
	bool Renderer::Init(const ApplicationSettings& settings)
	{
		// Acquire Resource
		m_Window = glfwGetCurrentContext();

		// load all OpenGL function pointers with glad
		// without it not all the OpenGL functions will be available,
		// such as glGetString(GL_RENDERER), and application might just segfault
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
			return false;
		}
		else
		{
			std::cout << "[INFO] GLAD initialized" << std::endl;
		}

		std::cout << "[INFO] OpenGL renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "[INFO] OpenGL from glad "
			<< GLVersion.major << "." << GLVersion.minor
			<< std::endl;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		return true;
	}

	void Renderer::Shutdown()
	{
		m_Window = nullptr;
	}

	void Renderer::Clear(glm::vec3 clearColor)
	{
		glClearColor(clearColor.r, clearColor.g, clearColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::Clear()
	{
		// the frame starts with a clean scene
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Renderer::Present()
	{
		glfwSwapBuffers(m_Window);
	}

	void Renderer::SetWireFrame(bool wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, (wireframe ? GL_LINE : GL_FILL));
	}

	void Renderer::Render(Mesh* mesh, Shader* shader)
	{
		shader->Bind();
		VertexArray* va = mesh->GetVAO();
		if (!va)
		{
			return;
		}
		va->Bind();
		IndexBuffer* ib = mesh->GetIBO();
		if (!ib)
		{
			return;
		}
		ib->Bind();

		glDrawElements(mesh->GetRenderMode(), ib->GetCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::Render(Mesh* mesh, Shader* shader, Light* light, Camera* camera, 
		Texture* diffuse, Texture* specular, Texture* normalMap)
	{
		diffuse->Bind(0);
		specular->Bind(1);
		normalMap->Bind(2);

		shader->SetUniformMatrix4f("uModel", mesh->transform.GetModelMatrix());
		shader->SetUniformMatrix4f("uProjection", camera->GetProjection());
		shader->SetUniformMatrix4f("uView", camera->GetView());

		shader->SetUniformInt("uMaterial.diffuse", 0);
		shader->SetUniformInt("uMaterial.specular", 1);
		shader->SetUniformInt("uMaterial.normalMap", 2);
		shader->SetUniformFloat("uMaterial.shininess", 32.0f);

		shader->SetUniformVec3("uLight.ambient", light->Ambient);
		shader->SetUniformVec3("uLight.diffuse", light->Diffuse); // darken diffuse light a bit
		shader->SetUniformVec3("uLight.specular", light->Specular);

		shader->SetUniformVec3("uLightPosition", light->transform.GetPosition());
		shader->SetUniformVec3("uCameraPosition", camera->GetViewPosition());

		Render(mesh, shader);
	}

	void Renderer::Render(Mesh* mesh, Shader* shader, Light* light, Camera* camera, 
		Texture* albedoMap, Texture* metallicMap, Texture* normalMap, Texture* roughnessMap, Texture* aoMap)
	{
		albedoMap->Bind(0);
		metallicMap->Bind(1);
		normalMap->Bind(2);
		roughnessMap->Bind(3);
		aoMap->Bind(4);

		shader->SetUniformMatrix4f("uModel", mesh->transform.GetModelMatrix());
		shader->SetUniformMatrix4f("uProjection", camera->GetProjection());
		shader->SetUniformMatrix4f("uView", camera->GetView());

		shader->SetUniformInt("uAlbedoMap", 0);
		shader->SetUniformInt("uMetallicMap", 1);
		shader->SetUniformInt("uNormalMap", 2);
		shader->SetUniformInt("uRoughnessMap", 3);
		shader->SetUniformInt("uAOMap", 4);

		shader->SetUniformVec3("uCameraPosition", camera->GetViewPosition());

		Render(mesh, shader);
	}
}