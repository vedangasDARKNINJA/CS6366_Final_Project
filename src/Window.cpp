#include "Window.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RendererPBR.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"

namespace RendererPBR
{
	static void glfw_error_callback(int error, const char* description)
	{
		std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	static void mouse_cursor_callback(GLFWwindow* window, double xPos, double yPos)
	{
		GEngine.GetMouse()->SetMouseCursorPos(xPos, yPos);
	}

	static void mouse_scroll_callback(GLFWwindow* window, double xScroll, double yScroll)
	{
		GEngine.GetMouse()->SetWheel(yScroll);
	}

	bool Window::Init(const ApplicationSettings& settings)
	{
		m_CloseOnEscape = settings.CloseApplicationOnEscape;
		glfwSetErrorCallback(glfw_error_callback);

		if (!glfwInit())
		{
			std::cerr << "[ERROR] Couldn't initialize GLFW" << std::endl;
			return false;
		}
		else
		{
			std::cout << "[INFO] GLFW initialized" << std::endl;
		}

		glfwWindowHint(GLFW_DOUBLEBUFFER, 1);
		glfwWindowHint(GLFW_DEPTH_BITS, 24);
		glfwWindowHint(GLFW_STENCIL_BITS, 8);
		glfwWindowHint(GLFW_RESIZABLE, 0);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(
			settings.ApplicationWidth,
			settings.ApplicationHeight,
			settings.ApplicationTitle,
			0, //monitor
			0
		);

		if (!m_Window)
		{
			std::cerr << "[ERROR] Couldn't create a GLFW window" << std::endl;
			return false;
		}

		glfwSetWindowPos(m_Window, 100, 100);

		// Callbacks
		glfwSetFramebufferSizeCallback(m_Window, framebuffer_size_callback);
		glfwSetCursorPosCallback(m_Window, mouse_cursor_callback);
		glfwSetScrollCallback(m_Window, mouse_scroll_callback);

		glfwMakeContextCurrent(m_Window);

		// VSync
		glfwSwapInterval(1);

		std::cout << "[INFO] OpenGL from GLFW "
			<< glfwGetWindowAttrib(m_Window, GLFW_CONTEXT_VERSION_MAJOR)
			<< "."
			<< glfwGetWindowAttrib(m_Window, GLFW_CONTEXT_VERSION_MINOR)
			<< std::endl;

		return true;
	}

	void Window::Shutdown()
	{
		if (m_Window != nullptr)
		{
			glfwDestroyWindow(m_Window);
		}
		glfwTerminate();
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();
		if (m_CloseOnEscape && GEngine.GetKeyboard()->IsKeyDown(GLFW_KEY_ESCAPE))
		{
			std::cout << "Quitting" << std::endl;
			glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
		}
	}

	int Window::GetWindowWidth() const
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		return width;
	}

	int Window::GetWindowHeight() const
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		return height;
	}

	bool Window::IsWindowOpen() const
	{
		return !glfwWindowShouldClose(m_Window);
	}
}
