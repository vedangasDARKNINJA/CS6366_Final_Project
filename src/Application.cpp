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
#include "AssimpModel.hpp"

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
	AssimpModel* sphere = nullptr;
	TestQuad* quad = nullptr;

	Shader* textureShader = nullptr;
	Shader* pbrDirectLighting = nullptr;
	Shader* pbrImageBasedLighting = nullptr;

	Framebuffer* framebuffer = nullptr;
	Renderbuffer* renderbuffer = nullptr;
	Texture* fbTexture = nullptr;
	Shader* fbShader = nullptr;
	FBQuad* fbQuad = nullptr;

	// PBR Textures
	Texture* diffuseTexture = nullptr;
	Texture* specularTexture = nullptr;
	Texture* metallicTexture = nullptr;
	Texture* normalMapTexture = nullptr;
	Texture* roughnessMapTexture = nullptr;
	Texture* aoMapTexture = nullptr;

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
		delete quad;
		delete light;
		delete textureShader;
		delete pbrDirectLighting;
		delete pbrImageBasedLighting;

		delete diffuseTexture;
		delete specularTexture;
		delete normalMapTexture;
		delete metallicTexture;
		delete roughnessMapTexture;
		delete aoMapTexture;

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

		pbrDirectLighting = new Shader("data/shaders/PBR_direct.shader");
		if (!pbrDirectLighting->WasCompilationSuccessful())
		{
			__debugbreak();
		}

		diffuseTexture = new Texture("data/textures/albedo.png", true);
		specularTexture = new Texture("data/textures/height.png");
		normalMapTexture = new Texture("data/textures/normals.png");
		metallicTexture = new Texture("data/textures/metallic.png");
		roughnessMapTexture = new Texture("data/textures/roughness.png");
		aoMapTexture = new Texture("data/textures/ao.png");

		light = new Light();
		light->transform.SetPosition({ 1.5f, 2.0f, 1.0f });

		sphere = new AssimpModel("data/models/sphere.obj");
		quad = new TestQuad();
		quad->transform.SetRotation({ 60.0f, 0.0f, 0.0f });

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
		Shader* currentShader = textureShader;

		static const char* modelNames[] = { "Sphere", "Quad" };
		static const char* currentModelName = modelNames[0];
		static Mesh* const models[] = { sphere, quad };

		static const char* shaderNames[] = { "Texture Shader", "PBR Direct Lighting" };
		static const char* currentShaderName = shaderNames[0];
		static Shader* const shaders[] = { textureShader, pbrDirectLighting };

		static glm::vec3 lightOffsets[4] = {
			glm::vec3(0.0f,0.5f,0.5f),
			glm::vec3(0.0f,-0.5f,0.5f),
			glm::vec3(0.0f,0.5f,-0.5f),
			glm::vec3(0.0f,-0.75f,-0.5f),
		};

		static glm::vec3 lightPositions[4] = {
			glm::vec3(-1.0f,1.0f,0.5f),
			glm::vec3(2.0f,1.5f,1.5f),
			glm::vec3(-1.0f,1.1f,1.5f),
			glm::vec3(0.5f,-1.0f,1.0f),
		};

		static glm::vec3 lightColors[4] = {
			glm::vec3(1.0f,1.0f,1.0f),
			glm::vec3(1.0f,0.0f,1.0f),
			glm::vec3(0.68f,0.68f,0.68f),
			glm::vec3(0.0f,1.0f,1.0f),
		};

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


			if (currentShader == textureShader)
			{
				m_Renderer->Render(currentMesh, currentShader, light, orbitCamera,
					diffuseTexture, specularTexture, normalMapTexture);
			}
			else
			{

				const int rows = 7;
				const int cols = 7;
				const float spacing = 2.5f;

				// render grid of spheres with varying metallic/roughness values scaled by rows and columns respectively
				glm::mat4 model = glm::mat4(1.0f);
				for (int row = 0; row < rows; ++row)
				{
					currentShader->SetUniformFloat("uMetallic", (float)row / (float)rows, true);
					for (int col = 0; col < cols; ++col)
					{
						currentShader->SetUniformFloat("uRoughness", glm::clamp((float)col / (float)cols , 0.05f, 1.0f), true);
						glm::vec3 positionOffset((col - (cols / 2)) * spacing, (row - (rows / 2)) * spacing, 0.0f);
						currentMesh->transform.SetPosition(positionOffset);
						for (int i = 0; i < 4; ++i)
						{
							lightOffsets[i] = lightPositions[i] + positionOffset;
						}
						currentShader->SetUniformVec3Array("uLightPositions", &lightOffsets[0][0], 4);
						currentShader->SetUniformVec3Array("uLightColors", &lightColors[0][0], 4);
						m_Renderer->Render(currentMesh, currentShader, light, orbitCamera,
							diffuseTexture, metallicTexture, normalMapTexture, roughnessMapTexture, aoMapTexture);
					}
				}
			}

			framebuffer->Unbind();
			glDisable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

			// Normal Render pass
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

			m_ImGuiSubsystem->NewFrame();
			currentShader->OnImGuiDraw();
			ImGui::Begin("Lights");
			{
				if (currentShader == textureShader)
				{
					light->OnImGuiDraw();
				}
				else
				{
					for (int i = 0; i < 4; ++i)
					{
						std::string label = "LightPosition" + i;
						std::string color = "LightColor" + i;
						ImGui::DragFloat3(label.c_str(), &lightPositions[i][0]);
						ImGui::ColorEdit3(color.c_str(), &lightColors[i][0]);
						ImGui::Dummy(ImVec2(0.0f, 2.0f));
					}
				}
			}
			ImGui::End();

			ImGui::Begin("Inspector");
			{
				orbitCamera->OnImGuiDraw();
				currentMesh->OnImGuiDraw();

				static bool wireFrame = false;
				if (ImGui::Checkbox("Wireframe Mode", &wireFrame))
				{
					m_Renderer->SetWireFrame(wireFrame);
				}

				ImGui::SeparatorText("Models");
				if (ImGui::BeginCombo("Rendering Model", currentModelName))
				{
					for (int n = 0; n < IM_ARRAYSIZE(modelNames); n++)
					{
						bool is_selected = (currentModelName == modelNames[n]);
						if (ImGui::Selectable(modelNames[n], &is_selected))
						{
							currentModelName = modelNames[n];
							currentMesh = models[n];
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
					}
					ImGui::EndCombo();
				}

				ImGui::SeparatorText("Shaders");
				if (ImGui::BeginCombo("Current Shader", currentShaderName))
				{
					for (int n = 0; n < IM_ARRAYSIZE(shaderNames); n++)
					{
						bool is_selected = (currentShaderName == shaderNames[n]);
						if (ImGui::Selectable(shaderNames[n], &is_selected))
						{
							currentShaderName = shaderNames[n];
							currentShader = shaders[n];
							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
					}
					ImGui::EndCombo();
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