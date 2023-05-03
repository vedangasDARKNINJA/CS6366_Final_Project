#pragma once 
#include "Mesh.hpp"

namespace RendererPBR
{
	class Light : public Mesh
	{
	public:
		Light();
		virtual const char* GetName()const override { return "Light"; }
		virtual unsigned int GetRenderMode() const override;

		void SetColor(glm::vec3 lightColor);
		glm::vec4 GetColor()const;
	private:
		glm::vec3 m_LightColor;
	};
}