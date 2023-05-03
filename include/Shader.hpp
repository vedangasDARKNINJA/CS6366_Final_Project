#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace RendererPBR
{
	class Shader
	{
	public:
		Shader(const std::string& path);
		~Shader();
		void Bind() const;
		void Unbind() const;

		void SetUniformMatrix4f(const std::string& name, const glm::mat4& data);

		void SetUniformVec4Array(const std::string& name, const float* data, size_t size);
		void SetUniformVec4(const std::string& name, float x, float y, float z, float w);
		void SetUniformVec4(const std::string& name, glm::vec4 vector);

		void SetUniformVec3Array(const std::string& name, const float* data, size_t size);
		void SetUniformVec3(const std::string& name, float x, float y, float z);
		void SetUniformVec3(const std::string& name, glm::vec3 vector);

		void SetUniformFloat(const std::string& name, float x);
		void SetUniformInt(const std::string& name, int value);

		bool WasCompilationSuccessful()const { return m_CompilationSuccess; }
	private:
		// Compilation and Linking
		bool CompileShader(uint32_t& shaderID, uint32_t shaderType, const std::string& shaderSource);
		bool LinkShaders(uint32_t vertexShaderID, uint32_t fragmentShaderID);

		//Uniforms
		uint32_t GetUniformLocation(const std::string& name);

	private:
		bool m_CompilationSuccess = false;
		std::string m_ShaderPath;
		std::unordered_map<std::string, uint32_t> m_UniformLocationMap;
		uint32_t m_ProgramID = 0;
	};
}