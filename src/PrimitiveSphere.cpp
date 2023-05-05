#include "PrimitiveSphere.hpp"
#include <vector>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

namespace RendererPBR
{
	PrimitiveSphere::PrimitiveSphere(unsigned int rings, unsigned int divisions)
	{
		const float radius = 1.0f;

		const float horizontalAngleStep = 360.0f / divisions;
		const float verticalAngleStep = 180.0f / rings;


		/*
		* Code from: http://www.songho.ca/opengl/gl_sphere.html
		*/
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
		float s, t;                                     // texCoord

		const float tilingX = 4.0f;
		const float tilingY = 2.0f;

		const float divisionInv = 1.0f /  divisions;
		const float ringsInv = 1.0f / rings;
		constexpr const float PI = glm::radians(180.0f);
		float sectorStep = glm::radians(360.0f / divisions);
		float stackStep = glm::radians(180.0f / rings);
		float sectorAngle, stackAngle;

		for (int i = 0; i <= rings; ++i)
		{
			stackAngle = (0.5f * PI) - i * stackStep;   // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= divisions; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				// normalized vertex normal
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				vertices.push_back(nx);
				vertices.push_back(ny);
				vertices.push_back(nz);

				// vertex tex coord between [0, 1]
				s = (float)tilingX * j * divisionInv;
				t = (float)tilingY * i * ringsInv;
				vertices.push_back(s);
				vertices.push_back(t);
			}
		}

		// indices
		indices.clear();

		int k1, k2;
		for (int i = 0; i < rings; ++i)
		{
			k1 = i * (divisions + 1);     // beginning of current stack
			k2 = k1 + divisions + 1;      // beginning of next stack

			for (int j = 0; j <= divisions; ++j, ++k1, ++k2)
			{
				indices.push_back(k1);
				indices.push_back(k2);
			}
			// NOTE: no need to add invisible triangles for sphere
			// because the triangle strip is wrapped and connected
			//@@ add 2 swaps (invisible triangles)
			//@@indices.push_back(k2-1);    // the last vertex in the current row
			//@@indices.push_back(k2);      // the first vertex in the next row
		}

		m_VertexArray = new VertexArray();
		m_VertexArray->Bind();

		VertexBuffer vb(vertices.data(), vertices.size());
		m_IndexBuffer = new IndexBuffer(indices.data(), indices.size());

		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(3); // Normals
		layout.Push<float>(2); // Tex Coordinates

		m_VertexArray->AddBuffer(vb, layout);

		m_VertexArray->Unbind();
		m_IndexBuffer->Unbind();
	}

	unsigned int PrimitiveSphere::GetRenderMode() const
	{
		return GL_TRIANGLE_STRIP;
	}
}