#pragma once
#include <vector>
#include "ApplicationSettings.hpp"

namespace RendererPBR
{
	class Subsystem;
	class Window;
	class Renderer;
	class Keyboard;
	class Mouse;
	class ImGuiSubsystem;

	class Application
	{
	private:
	public:
		static Application& Instance();
		void Init(const ApplicationSettings& settings);
		void Run();
		void Shutdown();

		void Quit();

		Window* GetWindow() const;
		Renderer* GetRenderer() const;
		Keyboard* GetKeyboard() const;
		Mouse* GetMouse() const;
		ImGuiSubsystem* GetImGui() const;
	private:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		bool m_IsRunning = false;

		Window* m_Window = nullptr;
		Renderer* m_Renderer= nullptr;
		Keyboard* m_Keyboard = nullptr;
		Mouse* m_Mouse = nullptr;
		ImGuiSubsystem* m_ImGuiSubsystem = nullptr;

		std::vector<Subsystem*> m_SubSystems;
	};
}