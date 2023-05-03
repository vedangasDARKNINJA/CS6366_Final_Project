#pragma once
#include <string>

namespace RendererPBR
{
	class Texture
	{
	public:
		Texture(const std::string& path, bool sRGB = false);
		Texture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data, bool sRGB = false);
		~Texture();

		void Bind(unsigned int slot = 0)const;

		inline int GetWidth()const { return m_Width; }
		inline int GetHeight()const { return m_Height; }
		inline int GetTextureID()const { return m_RendererID; }
	private:
		unsigned int m_RendererID;
		std::string m_Filepath;
		unsigned int m_Width;
		unsigned int m_Height;
		unsigned int m_Channels;
	};
}