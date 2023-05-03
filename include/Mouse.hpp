#pragma once
#include "Subsystem.hpp"
#include <glm/glm.hpp>

class GLFWwindow;

namespace RendererPBR
{
	class Mouse :public Subsystem
	{
	public:
		virtual bool Init(const ApplicationSettings& settings)override;
		virtual void Shutdown()override;

		void ProcessEvents();
		void SetWheel(double wheelValue);
		void SetMouseCursorPos(double mouseX, double mouseY);
		void SetMouseCursorPos(glm::dvec2 mousePosition);

		bool IsMouseButtonDown(int mouseButton) const;
		bool IsMouseButtonUp(int mouseButton) const;
		
		glm::dvec2 GetCursorPosition()const;
		glm::dvec2 GetDeltaCursorPosition()const;
		float GetDeltaWheel()const;

	private:
		GLFWwindow* m_Window = nullptr;

		double m_NewWheel = 0.0f;
		double m_DeltaWheel = 0.0f;

		double m_DeltaX = 0.0f;
		double m_DeltaY = 0.0f;

		double m_NewX = 0.0f;
		double m_NewY = 0.0f;

		double m_LastX = 0.0f;
		double m_LastY = 0.0f;
	};
}