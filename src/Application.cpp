#include <iostream>
#include "Application.hpp"
#include "Subsystem.hpp"
#include "Window.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "ImGuiSubsystem.hpp"
#include "Transform.hpp"
#include "Camera.hpp"
#include "PrimitiveSphere.hpp"
#include "Light.hpp"
#include "Framebuffer.hpp"
#include "Renderbuffer.hpp"
#include "TestQuad.hpp"
#include "FBQuad.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>

namespace RendererPBR
{
	Light* light = nullptr;
	Shader* textureShader = nullptr;
	Texture* whiteTexture = nullptr;
	PrimitiveSphere* sphere = nullptr;
	TestQuad* quad = nullptr;

	Framebuffer* framebuffer = nullptr;
	Renderbuffer* renderbuffer = nullptr;
	Texture* fbTexture = nullptr;
	Shader* fbShader = nullptr;
	FBQuad* fbQuad = nullptr;

	// PBR Textures
	Texture* diffuseTexture = nullptr;
	Texture* specularTexture = nullptr;

	Application::Application()
	{
		m_Window = new Window();
		m_Keyboard = new Keyboard();
		m_Mouse = new Mouse();
		m_Renderer = new Renderer();
		m_ImGuiSubsystem = new ImGuiSubsystem();

		m_SubSystems.push_back(m_Window);
		m_SubSystems.push_back(m_Renderer);
		m_SubSystems.push_back(m_Keyboard);
		m_SubSystems.push_back(m_Mouse);
		m_SubSystems.push_back(m_ImGuiSubsystem);
	}

	Application::~Application()
	{
		delete sphere;
		delete textureShader;
		delete whiteTexture;
		delete light;
		delete quad;

		delete diffuseTexture;
		delete specularTexture;

		delete framebuffer;
		delete renderbuffer;
		delete fbShader;
		delete fbTexture;
		delete fbQuad;

		delete m_Window;
		delete m_ImGuiSubsystem;
		delete m_Keyboard;
		delete m_Mouse;
		delete m_Renderer;
	}

	Application& Application::Instance()
	{
		static Application m_Instance;
		return m_Instance;
	}


	void Application::Init(const ApplicationSettings& settings)
	{
		m_IsRunning = true;
		for (auto& system : m_SubSystems)
		{
			if (!system->Init(settings))
			{
				m_IsRunning = false;
				break;
			}
		}

		textureShader = new Shader("data/shaders/texture.shader");
		if (!textureShader->WasCompilationSuccessful())
		{
			__debugbreak();
		}

		diffuseTexture = new Texture("data/textures/albedo.png");
		specularTexture = new Texture("data/textures/roughness.png");

		unsigned char data[] = { 0xFF, 0xFF, 0xFF, 0xFF };
		whiteTexture = new Texture(1, 1, 4, data);

		light = new Light();
		light->transform.SetPosition({ 2.0f, 1.5f, 0.5f });
		light->SetColor({ 0.988f, 0.655f, 0.337f });

		quad = new TestQuad();

		sphere = new PrimitiveSphere();

		framebuffer = new Framebuffer();
		const unsigned int width = m_Window->GetWindowWidth(), height = m_Window->GetWindowHeight();
		renderbuffer = new Renderbuffer(width, height);
		fbShader = new Shader("data/shaders/framebuffer.shader");
		if (!fbShader->WasCompilationSuccessful())
		{
			__debugbreak();
		}
		fbTexture = new Texture(width, height, 3, nullptr);
		fbQuad = new FBQuad();

		framebuffer->Bind();
		framebuffer->AttachTexture(fbTexture);
		framebuffer->AttachRenderBuffer(renderbuffer);
		framebuffer->Unbind();
	}

	void Application::Run()
	{
		Camera* orbitCamera = new OrbitalCamera();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		double prevFrameTime = glfwGetTime();
		double currFrameTime = glfwGetTime();
		double deltaTime = glfwGetTime();

		std::cout << "FB Status: " << framebuffer->IsComplete() << std::endl;

		Mesh* currentMesh = sphere;
		while (m_IsRunning && m_Window->IsWindowOpen())
		{
			prevFrameTime = currFrameTime;
			currFrameTime = glfwGetTime();
			deltaTime = currFrameTime - prevFrameTime;

			// Render to FB
			framebuffer->Bind();
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

			// make sure we clear the framebuffer's content
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/*
			texture1->Bind(1);
			textureShader->SetUniformMatrix4f("uModel", currentMesh->transform.GetModelMatrix());
			textureShader->SetUniformMatrix4f("uProjection", orbitCamera->GetProjection());
			textureShader->SetUniformMatrix4f("uView", orbitCamera->GetView());
			textureShader->SetUniformVec4("uTint", glm::vec4(1.0f));
			textureShader->SetUniformInt("uAlbedo", 1);

			// Lighting
			textureShader->SetUniformVec3("uViewPos", orbitCamera->GetViewPosition());
			textureShader->SetUniformVec3("uLightPos", light->transform.GetPosition());
			textureShader->SetUniformVec3("uLightColor", light->GetColor());
			*/
			m_Renderer->Render(currentMesh, textureShader, light, orbitCamera, diffuseTexture, specularTexture);

			framebuffer->Unbind();
			glDisable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

			// Normal Render pass
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

			m_ImGuiSubsystem->NewFrame();

			ImGui::Begin("Inspector");
			{
				orbitCamera->OnImGuiDraw();
				currentMesh->OnImGuiDraw();

				static bool wireFrame = false;
				if (ImGui::Checkbox("Wireframe Mode", &wireFrame))
				{
					m_Renderer->SetWireFrame(wireFrame);
				}
			}
			ImGui::End();

			fbTexture->Bind(1);
			fbShader->SetUniformInt("uScreenTexture", 1);
			m_Renderer->Render(fbQuad, fbShader);


			m_ImGuiSubsystem->Render();
			m_Renderer->Present();
			m_Window->ProcessEvents();
			m_Mouse->ProcessEvents();

			orbitCamera->ProcessEvents();
		}


		delete orbitCamera;
	}

	void Application::Shutdown()
	{
		for (auto itr = m_SubSystems.rbegin(), end = m_SubSystems.rend(); itr != end; ++itr)
		{
			(*itr)->Shutdown();
		}
		m_SubSystems.clear();
	}

	void Application::Quit()
	{
		m_IsRunning = false;
	}

	Window* Application::GetWindow() const
	{
		return m_Window;
	}

	Renderer* Application::GetRenderer() const
	{
		return m_Renderer;
	}

	Keyboard* Application::GetKeyboard() const
	{
		return m_Keyboard;
	}

	Mouse* Application::GetMouse() const
	{
		return m_Mouse;
	}

	ImGuiSubsystem* Application::GetImGui() const
	{
		return m_ImGuiSubsystem;
	}
}