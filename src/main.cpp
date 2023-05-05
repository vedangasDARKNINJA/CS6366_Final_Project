#include "Application.hpp"


int main(int argc, char *argv[])
{
    RendererPBR::ApplicationSettings settings;
    settings.ApplicationWidth = 1280;
    settings.ApplicationHeight = 720;
    settings.ApplicationTitle = "Renderer PBR";
    settings.CloseApplicationOnEscape = true;
    RendererPBR::Application::Instance().Init(settings);
    RendererPBR::Application::Instance().Run();
    RendererPBR::Application::Instance().Shutdown();
    return 0;
}
