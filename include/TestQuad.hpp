#pragma once
#include "Mesh.hpp"

namespace RendererPBR
{
	class TestQuad :public Mesh
	{
	public:
		TestQuad();

		virtual const char* GetName()const override { return "Quad"; }
		virtual unsigned int GetRenderMode()const override;
	};
}