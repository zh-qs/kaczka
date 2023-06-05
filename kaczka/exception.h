#pragma once

#include <stdexcept>
#include <string>

#define THROW_EXCEPTION throw GlApplicationInitException()

namespace kaczka
{
	class GlApplicationInitException : public std::exception {
	public:
		GlApplicationInitException() : exception("Application init error") { }
	};
}