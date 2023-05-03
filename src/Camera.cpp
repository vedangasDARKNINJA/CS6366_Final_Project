#include "Camera.hpp"
#include <iostream>

#include "RendererPBR.hpp"
#include "Mouse.hpp"

#include "GLFW/glfw3.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace RendererPBR
{
	Camera::Camera()
		:m_View(1.0f), m_Projection(1.0f), m_FOV(45.0f), m_NearPlane(0.1f), m_FarPlane(100.0f), m_ZoomSpeed(1.0f)
	{
		m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -5.0f));
		CalculateProjectionMatrix();
	}

	void Camera::SetProjection(float fov, float near, float far)
	{
		m_FOV = fov;
		m_NearPlane = near;
		m_FarPlane = far;
		GLFWwindow* window = glfwGetCurrentContext();
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		m_Projection = glm::perspective(glm::radians(fov), (float)width / (float)height, near, far);
	}

	void Camera::SetView(const glm::mat4& view)
	{
		m_View = view;
	}

	const glm::mat4& Camera::GetView() const
	{
		return  m_View;
	}

	const glm::mat4& Camera::GetProjection() const
	{
		return  m_Projection;
	}

	void Camera::OnImGuiDraw()
	{
		bool projectionChanges = false;
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Camera Settings");

		if (ImGui::DragFloat("Field of View", &m_FOV, 1.0f, -360.0f, 360.0f))
		{
			projectionChanges = true;
		}
		if (ImGui::DragFloat("Near Plane", &m_NearPlane, 1.0f, 0.1f, 100.0f))
		{
			projectionChanges = true;
		}

		if (ImGui::DragFloat("Far Plane", &m_FarPlane, 1.0f, 0.1, 500.0f))
		{
			projectionChanges = true;
		}

		ImGui::DragFloat("Zoom Speed", &m_ZoomSpeed, 1.0f, 0.1, 5.0f);

		if (projectionChanges)
		{
			CalculateProjectionMatrix();
		}
	}

	void Camera::CalculateProjectionMatrix()
	{
		SetProjection(m_FOV, m_NearPlane, m_FarPlane);
	}

	OrbitalCamera::OrbitalCamera()
		:Camera(), m_Angle(90.0f), m_LookAtPosition(0.0f), m_Radius(5.0f), m_Height(0.0f)
	{
		CalculateViewMatrix();
	}

	void OrbitalCamera::ProcessEvents()
	{
		bool enableCameraControl = false;
		if (GEngine.GetMouse()->IsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			enableCameraControl = true;
		}
		else
		{
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		const float wheel = GEngine.GetMouse()->GetDeltaWheel();
		if (wheel != 0.0f)
		{
			m_FOV -= m_ZoomSpeed * wheel;
			CalculateProjectionMatrix();
		}

		if (enableCameraControl)
		{
			glm::dvec2 deltaMouse = GEngine.GetMouse()->GetDeltaCursorPosition();
			m_Angle += m_Sensitivity * deltaMouse.x;
			m_Height += 0.5f * m_Sensitivity * deltaMouse.y;

			m_Height = std::fmin(std::fmax(m_Height, -0.99f * m_Radius), 0.99f * m_Radius);

			while (m_Angle > 360.0f)
			{
				m_Angle -= 360.0f;
			}
			while (m_Angle < -360.0f)
			{
				m_Angle += 360.0f;
			}

			CalculateViewMatrix();
		}
	}

	void OrbitalCamera::OnImGuiDraw()
	{
		ImGui::SeparatorText("Orbit Camera Controls");
		Camera::OnImGuiDraw();

		bool viewChanges = false;
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Orbit Settings");
		ImGui::DragFloat("Sensitivity", &m_Sensitivity, 0.1f, 0.1f, 1.0f);

		if (ImGui::DragFloat("Radius", &m_Radius, 0.1f, 0.1f))
		{
			viewChanges = true;
		}

		if (ImGui::DragFloat("Height", &m_Height, 0.1f))
		{
			viewChanges = true;
		}

		if (ImGui::DragFloat("Angle", &m_Angle, 1.0f, -360.0f, 360.0f))
		{
			viewChanges = true;
		}

		if (ImGui::DragFloat3("Look At", &m_LookAtPosition[0], 1.0f))
		{
			viewChanges = true;
		}

		if (viewChanges)
		{
			CalculateViewMatrix();
		}
	}

	const glm::vec3& OrbitalCamera::GetViewPosition() const
	{
		return m_Position;
	}

	void OrbitalCamera::CalculateViewMatrix()
	{
		const float angle = glm::asin(m_Height / m_Radius);
		const float newRadius = m_Radius * glm::cos(angle);
		const glm::vec3 positionOffset(newRadius * glm::cos(glm::radians(m_Angle)), m_Height, newRadius * glm::sin(glm::radians(m_Angle)));
		m_Position = m_LookAtPosition + positionOffset;
		m_View = glm::lookAt(m_LookAtPosition + positionOffset, m_LookAtPosition, glm::vec3(0.0f, 1.0f, 0.0f));
	}

}