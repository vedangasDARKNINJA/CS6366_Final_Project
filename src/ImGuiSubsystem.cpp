#include "ImGuiSubsystem.hpp"
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include <imgui_impl_opengl3.cpp>
#include <imgui_impl_glfw.cpp>

namespace RendererPBR
{
	bool ImGuiSubsystem::Init(const ApplicationSettings& settings)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		if (!ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true))
		{
			return false;
		}
		if (!ImGui_ImplOpenGL3_Init())
		{
			return false;
		}

		std::cout << "[SUCCESS]: ImGui Initialized Sucessfully!" << std::endl;

		return true;
	}

	void ImGuiSubsystem::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiSubsystem::NewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiSubsystem::Render()
	{
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}