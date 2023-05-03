#pragma once
#include <vector>


namespace RendererPBR
{
	struct ApplicationSettings 
	{
		const char* ApplicationTitle = "Renderer PBR";
		int32_t ApplicationWidth = 800;
		int32_t ApplicationHeight = 600;

		bool CloseApplicationOnEscape = false;
	};
}