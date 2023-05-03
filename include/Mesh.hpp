#pragma once
#include "Transform.hpp"

namespace RendererPBR
{
	class Shader;
	class VertexArray;
	class IndexBuffer;
	class Renderer;

	class Mesh
	{
	public:
		Mesh();
		virtual ~Mesh();

		virtual const char* GetName()const = 0;
		virtual unsigned int GetRenderMode()const = 0;

		VertexArray* GetVAO() const;
		IndexBuffer* GetIBO() const;

		virtual void OnImGuiDraw();

		Transform transform;
	protected:
		VertexArray* m_VertexArray = nullptr;
		IndexBuffer* m_IndexBuffer = nullptr;
	};
}