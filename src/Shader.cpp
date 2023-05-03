#include "Shader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace RendererPBR
{
	Shader::Shader(const std::string& path)
		:m_ShaderPath(path),m_CompilationSuccess(false)
	{
		m_ProgramID = glCreateProgram();
		enum class SHADER_TYPE :int32_t
		{
			NONE = -1,
			VERTEX_SHADER,
			FRAGMENT_SHADER,
			MAX_SHADER_TYPES
		};

		std::cout << "[INFO]: Compiling shader at [" << m_ShaderPath << "]\n";

		std::ifstream stream(m_ShaderPath);
		std::string line;

		std::stringstream shaderSources[(size_t)SHADER_TYPE::MAX_SHADER_TYPES];
		int currentShaderType = -1;

		while (getline(stream, line))
		{
			size_t position = line.find("#shader");
			if (position != std::string::npos)
			{
				if (line.find("vertex", position) != std::string::npos)
				{
					currentShaderType = (int32_t)SHADER_TYPE::VERTEX_SHADER;
				}
				else if (line.find("fragment", position) != std::string::npos)
				{
					currentShaderType = (int32_t)SHADER_TYPE::FRAGMENT_SHADER;
				}
			}
			else
			{
				if (currentShaderType == -1)
				{
					std::cerr << "[ERROR]: Shader code doesn't specify the type of the shader. Aborting." << std::endl;
					return;
				}

				shaderSources[currentShaderType] << line << '\n';
			}
		}

		uint32_t vertexShaderID = 0;
		if (!CompileShader(vertexShaderID, GL_VERTEX_SHADER, shaderSources[(size_t)SHADER_TYPE::VERTEX_SHADER].str()))
		{
			return;
		}
		uint32_t fragmentShaderID = 0;
		if (!CompileShader(fragmentShaderID, GL_FRAGMENT_SHADER, shaderSources[(size_t)SHADER_TYPE::FRAGMENT_SHADER].str()))
		{
			return;
		}
		if (!LinkShaders(vertexShaderID, fragmentShaderID))
		{
			return;
		}

		std::cout << "[SUCCESS]: Compiling and Linking shader at [" << m_ShaderPath << "] Successful!\n";
		m_CompilationSuccess = true;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ProgramID);
	}

	void Shader::Bind() const
	{
		if (m_ProgramID == 0)
		{
			std::cout << "[ERROR]: The shader program is not a valid program. Please check if this shader[" << m_ShaderPath << "] has any compilation/linking errors.";
		}
		glUseProgram(m_ProgramID);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniformVec4Array(const std::string& name, const float* data, size_t size)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniform4fv(uniformLocation, size, data);
	}

	void Shader::SetUniformVec4(const std::string& name, float x, float y, float z, float w)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniform4f(uniformLocation, x, y, z, w);
	}

	void Shader::SetUniformVec4(const std::string& name, glm::vec4 vector)
	{
		SetUniformVec4(name, vector[0], vector[1], vector[2], vector[3]);
	}

	void Shader::SetUniformFloat(const std::string& name, float x)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniform1f(uniformLocation, x);
	}

	void Shader::SetUniformInt(const std::string& name, int value)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniform1i(uniformLocation, value);
	}

	void Shader::SetUniformVec3Array(const std::string& name, const float* data, size_t size)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniform3fv(uniformLocation, size, data);
	}

	void Shader::SetUniformVec3(const std::string& name, float x, float y, float z)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniform3f(uniformLocation, x, y, z);
	}

	void Shader::SetUniformVec3(const std::string& name, glm::vec3 vector)
	{
		SetUniformVec3(name, vector[0], vector[1], vector[2]);
	}

	void Shader::SetUniformMatrix4f(const std::string& name, const glm::mat4& data)
	{
		Bind();
		uint32_t uniformLocation = GetUniformLocation(name);
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &data[0][0]);
	}

	/*
	* Adapted from the official Khronos OpenGL wiki.
	* https://www.khronos.org/opengl/wiki/Shader_Compilation#Example
	*/
	bool Shader::CompileShader(uint32_t& shaderID, uint32_t shaderType, const std::string& shaderSource)
	{
		const char* source = shaderSource.c_str();
		shaderID = glCreateShader(shaderType);
		glShaderSource(shaderID, 1, &source, 0);
		glCompileShader(shaderID);
		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength];
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, infoLog);

			glDeleteShader(shaderID);

			std::cout << "Shader Compilation Error\n[ERROR]:" << infoLog << std::endl;
			m_ProgramID = 0;

			delete[] infoLog;
			return false;
		}

		return true;
	}

	bool Shader::LinkShaders(uint32_t vertexShaderID, uint32_t fragmentShaderID)
	{
		if (m_ProgramID == 0)
		{
			std::cout << "[ERROR]: The shader program is not a valid program. Please check if this shader[" << m_ShaderPath << "] has any compilation/linking errors.";
		}
		glAttachShader(m_ProgramID, vertexShaderID);
		glAttachShader(m_ProgramID, fragmentShaderID);

		glLinkProgram(m_ProgramID);

		GLint isLinked = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			char* infoLog = new char[maxLength];
			glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, infoLog);

			glDeleteProgram(m_ProgramID);
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			std::cout << "Shader Linking Error\n[ERROR]:" << infoLog << std::endl;
			m_ProgramID = 0;

			delete[] infoLog;
			return false;
		}

		glDetachShader(m_ProgramID, vertexShaderID);
		glDetachShader(m_ProgramID, fragmentShaderID);
		return true;
	}

	uint32_t Shader::GetUniformLocation(const std::string& name)
	{
		if (m_ProgramID == 0)
		{
			std::cerr << "[ERROR]: Trying to set uniform for a shader that is not compiled." << std::endl;
			return 0;
		}

		if (m_UniformLocationMap.find(name) == m_UniformLocationMap.end())
		{
			int uniformLocation = glGetUniformLocation(m_ProgramID, name.c_str());
			if (uniformLocation == -1)
			{
				std::cout << "[WARNING]: The uniform doesn't exist in the shader [" << m_ShaderPath << "]" << std::endl;
				return -1;
			}

			m_UniformLocationMap.emplace(name, glGetUniformLocation(m_ProgramID, name.c_str()));
		}

		return m_UniformLocationMap.at(name);
	}
}