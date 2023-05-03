#pragma once
#include <glm/glm.hpp>
#include "Transform.hpp"

namespace RendererPBR
{
	class Camera
	{
	public:
		Camera();
		~Camera() = default;

		void SetProjection(float fov, float near, float far);
		void SetView(const glm::mat4& view);

		const glm::mat4& GetView() const;
		const glm::mat4& GetProjection() const;

		virtual const glm::vec3& GetViewPosition() const = 0;
		virtual void ProcessEvents() = 0;
		virtual void OnImGuiDraw();
	protected:
		void CalculateProjectionMatrix();

	protected:
		glm::mat4 m_View;
		glm::mat4 m_Projection;

		float m_FOV;
		float m_NearPlane;
		float m_FarPlane;
		float m_ZoomSpeed;
	};

	class OrbitalCamera :public Camera
	{
	public:
		OrbitalCamera();
		~OrbitalCamera() = default;

		virtual void ProcessEvents() override;
		virtual void OnImGuiDraw() override;
		virtual const glm::vec3& GetViewPosition() const override;

	private:
		void CalculateViewMatrix();

	private:
		glm::vec3 m_LookAtPosition;
		glm::vec3 m_Position;
		float m_Angle = 0.0f;
		float m_Radius = 5.0f;
		float m_Height = 5.0f;
		float m_Sensitivity = 0.2f;
	};
}
