#pragma once 
#include "ApplicationSettings.hpp"

namespace RendererPBR
{
	class Subsystem
	{
	public:
		virtual bool Init(const ApplicationSettings& settings) = 0;
		virtual void Shutdown() = 0;
	};
}