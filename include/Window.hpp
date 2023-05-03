#pragma once 
#include "Subsystem.hpp"

class GLFWwindow;

namespace RendererPBR
{
	class Window: public Subsystem
	{
	public:
		virtual bool Init(const ApplicationSettings& settings) override;
		virtual void Shutdown() override;

		/*
		* A call to poll events.
		*/
		void ProcessEvents();

		// Getters
		int GetWindowWidth() const;
		int GetWindowHeight() const;
		bool IsWindowOpen()const;

	private:
		bool m_CloseOnEscape = false;
		GLFWwindow* m_Window = nullptr;
	};
}