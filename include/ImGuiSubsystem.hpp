#pragma once
#include "Subsystem.hpp"

namespace RendererPBR
{
    class ImGuiSubsystem : public Subsystem
    {
    public:
        virtual bool Init(const ApplicationSettings& settings) override;
        virtual void Shutdown() override;

        void NewFrame();
        void Render();
    };
}