#pragma once 
#include <bitset>

#include "Subsystem.hpp"

class GLFWwindow;

namespace RendererPBR
{
	class Keyboard : public Subsystem
	{
	public:
		virtual bool Init(const ApplicationSettings& settings) override;
		virtual void Shutdown() override;

		// Key state getters
		bool IsKeyDown(int keycode) const;
		bool IsKeyUp(int keycode) const;

	private:
		GLFWwindow* m_Window = nullptr;
	};
}
