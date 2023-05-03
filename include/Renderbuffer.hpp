#pragma once

namespace RendererPBR
{
    class Texture;

    class Renderbuffer
    {
    public:
       Renderbuffer(unsigned int width, unsigned int height); 
       ~Renderbuffer();
       void Bind() const;
       void Unbind() const;

       unsigned int GetID()const;

    private:
        unsigned int m_RendererID;
        unsigned int m_Width;
        unsigned int m_Height;
    };
}