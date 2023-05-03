#pragma once

namespace RendererPBR
{
    class Texture;
    class Renderbuffer;

    class Framebuffer
    {
    public:
       Framebuffer(); 
       ~Framebuffer();
       bool IsComplete()const;
       void Bind() const;
       void Unbind() const;

       void AttachTexture(Texture* texture);
       void AttachRenderBuffer(Renderbuffer* renderbuffer);

       unsigned int GetID()const;

    private:
        unsigned int m_RendererID;
    };
}