#include "Keyboard.hpp"
#include <GLFW/glfw3.h>

namespace RendererPBR
{
	bool Keyboard::Init(const ApplicationSettings& settings)
	{
		m_Window = glfwGetCurrentContext();
		return true;
	}

	void Keyboard::Shutdown()
	{
		m_Window = nullptr;
	}

	bool Keyboard::IsKeyDown(int keycode) const
	{
		return glfwGetKey(m_Window, keycode) == GLFW_PRESS;
	}

	bool Keyboard::IsKeyUp(int keycode) const
	{
		return glfwGetKey(m_Window, keycode) == GLFW_RELEASE;
	}
}