#include "Transform.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace RendererPBR
{
	Transform::Transform()
		:m_Translation(0.0f),m_EulerAngles(0.0f),m_Scale(1.0f), m_CachedModelMatrix(1.0f),m_IsDirty(true)
	{
	}
	void Transform::SetPosition(glm::vec3 position)
	{
		m_Translation = position;
	}

	void Transform::SetScale(glm::vec3 scale)
	{
		m_Scale = scale;
	}

	void Transform::SetRotation(glm::vec3 eulerAngles)
	{
		m_EulerAngles = eulerAngles;
	}

	void Transform::OnImGuiDraw(const char* label)
	{
		ImGui::SeparatorText(label);
		if (ImGui::DragFloat3("Position", &m_Translation[0], 1.0f))
		{
			m_IsDirty = true;
		}
		if (ImGui::DragFloat3("Rotation", &m_EulerAngles[0], 1.0f, -360.0f, 360.0f))
		{
			m_IsDirty = true;
		}
		if (ImGui::DragFloat3("Scale", &m_Scale[0], 1.0f))
		{
			m_IsDirty = true;
		}

		if (m_IsDirty)
		{
			CalculateModelMatrix();
		}
	}

	glm::mat4 Transform::GetModelMatrix() const
	{
		if (m_IsDirty)
		{
			CalculateModelMatrix();
		}

		return m_CachedModelMatrix;
	}

	glm::vec3 Transform::GetPosition() const
	{
		return m_Translation;
	}

	glm::vec3 Transform::GetScale() const
	{
		return m_Scale;
	}

	glm::vec3 Transform::GetEulerAngles() const
	{
		return m_EulerAngles;
	}

	void Transform::CalculateModelMatrix() const
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
		glm::mat4 rotation = glm::eulerAngleXYZ(glm::radians(m_EulerAngles[0]),glm::radians(m_EulerAngles[1]),glm::radians(m_EulerAngles[2]));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_Translation);

		m_CachedModelMatrix = translate * rotation * scale;
		m_IsDirty = false;
	}
}