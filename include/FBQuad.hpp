#pragma once
#include "Mesh.hpp"

namespace RendererPBR
{
	class FBQuad :public Mesh
	{
	public:
		FBQuad();

		virtual const char* GetName()const override { return "FrameBuffer Quad"; }
		virtual unsigned int GetRenderMode()const override;
	};
}
