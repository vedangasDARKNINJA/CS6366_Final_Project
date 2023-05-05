#include "AssimpModel.hpp"
#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace RendererPBR
{
	AssimpModel::AssimpModel(const std::string& path)
	{

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "[ERROR] Error Loading model at [" << path << "]:\nError: " << importer.GetErrorString() << std::endl;
			return;
		}

		aiNode* root = scene->mRootNode;
		std::vector<aiMesh*> meshes;
		processNode(root, scene, meshes);
		for (const auto mesh : meshes)
		{
			processMesh(mesh, vertices, indices);
		}

		m_VertexArray = new VertexArray();
		m_VertexArray->Bind();

		std::cout << "==== Mesh Stats =====\n";
		std::cout << "no. of vertices: " << vertices.size() << "\n";
		std::cout << "no. indices: " << indices.size() << "\n";
		std::cout << "vertices size: " << vertices.size() * sizeof(Vertex) << "\n";

		VertexBuffer vb(vertices.data(), vertices.size() * sizeof(Vertex) / sizeof(float));
		m_IndexBuffer = new IndexBuffer(indices.data(), indices.size());

		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(3); // Normal
		layout.Push<float>(2); // Tex Coordinate
		layout.Push<float>(3); // Tangent

		m_VertexArray->AddBuffer(vb, layout);

		m_VertexArray->Unbind();
		m_IndexBuffer->Unbind();

	}

	unsigned int AssimpModel::GetRenderMode()const
	{
		return GL_TRIANGLES;
	}

	void AssimpModel::processNode(const aiNode* node, const aiScene* scene, std::vector<aiMesh*>& meshes)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(mesh);
		}

		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, meshes);
		}
	}

	void AssimpModel::processMesh(const aiMesh* meshData, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
	{
		for (unsigned int i = 0; i < meshData->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position = glm::vec3(
				meshData->mVertices[i].x,
				meshData->mVertices[i].y,
				meshData->mVertices[i].z
			);
			vertex.Normal = glm::vec3(
				meshData->mNormals[i].x,
				meshData->mNormals[i].y,
				meshData->mNormals[i].z
			);

			if (meshData->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				vertex.TexCoords = glm::vec2(
					meshData->mTextureCoords[0][i].x,
					meshData->mTextureCoords[0][i].y
				);
			}
			else
			{
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertex.Tangent = glm::vec3(
				meshData->mTangents[i].x,
				meshData->mTangents[i].y,
				meshData->mTangents[i].z
			);

			vertices.push_back(vertex);
		}

		for (unsigned int i = 0; i < meshData->mNumFaces; i++)
		{
			aiFace face = meshData->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}
	}
}