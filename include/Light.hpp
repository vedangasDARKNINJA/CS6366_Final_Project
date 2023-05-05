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


		virtual void OnImGuiDraw()override;

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;

	};
}