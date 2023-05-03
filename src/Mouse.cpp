#include "Mouse.hpp"
#include <GLFW/glfw3.h>

namespace RendererPBR
{
	bool Mouse::Init(const ApplicationSettings& settings)
	{
		m_Window = glfwGetCurrentContext();
		return true;
	}

	void Mouse::Shutdown()
	{
		m_Window = nullptr;
	}

	bool Mouse::IsMouseButtonDown(int mouseButton) const
	{
		return glfwGetMouseButton(m_Window, mouseButton);
	}

	bool Mouse::IsMouseButtonUp(int mouseButton) const
	{
		return !glfwGetMouseButton(m_Window, mouseButton);
	}

	void Mouse::ProcessEvents()
	{
		m_DeltaX = m_NewX - m_LastX;
		m_DeltaY = m_NewY - m_LastY;
		m_DeltaWheel = m_NewWheel;
		m_NewWheel = 0.0f;

		m_LastX = m_NewX;
		m_LastY = m_NewY;
	}

	void Mouse::SetWheel(double wheelValue)
	{
		m_NewWheel = wheelValue;
	}

	void Mouse::SetMouseCursorPos(double mouseX, double mouseY)
	{
		m_NewX = mouseX;
		m_NewY = mouseY;
	}

	void Mouse::SetMouseCursorPos(glm::dvec2 mousePosition)
	{
		SetMouseCursorPos(mousePosition.x, mousePosition.y);
	}

	glm::dvec2 Mouse::GetCursorPosition() const
	{
		return glm::dvec2(m_LastX, m_LastY);
	}

	glm::dvec2 Mouse::GetDeltaCursorPosition() const
	{
		return glm::dvec2(m_DeltaX, m_DeltaY);
	}
	float Mouse::GetDeltaWheel() const
	{
		return m_DeltaWheel;
	}
}