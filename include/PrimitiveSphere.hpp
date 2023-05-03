#pragma once

#include "Mesh.hpp"

namespace RendererPBR
{
	// Primitive Sphere of radius 1
	class PrimitiveSphere : public Mesh
	{
	public:
		PrimitiveSphere(unsigned int rings = 64, unsigned int divisions = 64);

		virtual const char* GetName() const override { return "PrimitiveSphere"; }
		virtual unsigned int GetRenderMode() const override;
	};
}