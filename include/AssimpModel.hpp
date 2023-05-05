#pragma once
#include <string>
#include <vector>
#include "Mesh.hpp"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace RendererPBR
{
    class AssimpModel :public Mesh
    {
    private:
		struct Vertex 
        {
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 TexCoords;
			glm::vec3 Tangent;
		};

    public:
        AssimpModel(const std::string& path);

        virtual const char* GetName()const override { return "Assimp Model"; }
        virtual unsigned int GetRenderMode()const override;

    private:
        void processNode(const aiNode* node, const aiScene* scene, std::vector<aiMesh*>& meshes);
        void processMesh(const aiMesh* meshData, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    };
}