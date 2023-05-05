#pragma once
#include <unordered_map>
#include <vector>
#include <set>
#include <string>
#include <glm/glm.hpp>

namespace RendererPBR
{
	class Shader
	{
		enum class UniformType :uint8_t
		{
			NONE = 0,
			FLOAT,
			BOOLEAN,
			FLOAT2,
			FLOAT3,
			COLOR3,
			FLOAT4,
			COLOR4
		};

		struct ShaderUniformSerialized
		{
			std::string uniformName;
			UniformType type;
			float data[4];
			size_t size;
		};

	public:
		Shader(const std::string& path);
		~Shader();
		void Bind() const;
		void Unbind() const;

		void SetUniformMatrix4f(const std::string& name, const glm::mat4& data, bool forceSet = false);

		void SetUniformVec4Array(const std::string& name, const float* data, size_t size, bool forceSet = false);
		void SetUniformVec4(const std::string& name, float x, float y, float z, float w, bool forceSet = false);
		void SetUniformVec4(const std::string& name, glm::vec4 vector, bool forceSet = false);

		void SetUniformVec3Array(const std::string& name, const float* data, size_t size, bool forceSet = false);
		void SetUniformVec3(const std::string& name, float x, float y, float z, bool forceSet = false);
		void SetUniformVec3(const std::string& name, glm::vec3 vector, bool forceSet = false);

		void SetUniformFloat(const std::string& name, float x, bool forceSet = false);
		void SetUniformInt(const std::string& name, int value, bool forceSet = false);

		bool WasCompilationSuccessful()const { return m_CompilationSuccess; }

		void OnImGuiDraw();

	private:
		// Compilation and Linking
		bool CompileShader(uint32_t& shaderID, uint32_t shaderType, const std::string& shaderSource);
		bool LinkShaders(uint32_t vertexShaderID, uint32_t fragmentShaderID);

		//Uniforms
		uint32_t GetUniformLocation(const std::string& name);
		void ParseLineForUniforms(const std::string& line);
		void AddUniformsToIgnoreList();

	private:
		bool m_CompilationSuccess = false;
		std::string m_ShaderPath;
		std::unordered_map<std::string, uint32_t> m_UniformLocationMap;
		uint32_t m_ProgramID = 0;

		//Serialization
		std::vector<ShaderUniformSerialized> m_SerializedShaderUniforms;
		std::set<unsigned int> m_IgnoredUniformLocations;
	};
}