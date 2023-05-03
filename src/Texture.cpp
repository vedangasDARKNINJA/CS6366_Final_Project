#include "Texture.hpp"
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

namespace RendererPBR
{
	Texture::Texture(const std::string& path, bool sRGB)
		:m_RendererID(0), m_Filepath(path), m_Width(0), m_Height(0), m_Channels(0)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			m_Width = width;
			m_Height = height;
			m_Channels = channels;
			unsigned int internalFormat = GL_RGB8;
			switch (m_Channels)
			{
			case 3:
			{
				internalFormat = sRGB ? GL_SRGB8 : GL_RGB8;
				break;
			}
			case 4:
			{
				internalFormat = sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				break;
			}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, (m_Channels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cerr << "[ERROR]: Error loading the texture at [" << path << std::endl;
		}

		stbi_image_free(data);
	}

	Texture::Texture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data, bool sRGB)
		:m_RendererID(0), m_Filepath(""), m_Width(width), m_Height(height), m_Channels(channels)
	{
		unsigned int internalFormat = GL_RGB8;
		switch (m_Channels)
		{
		case 3:
		{
			internalFormat = sRGB ? GL_SRGB8 : GL_RGB8;
			break;
		}
		case 4:
		{
			internalFormat = sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			break;
		}
		}
		
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, (m_Channels == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void Texture::Bind(unsigned int slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}