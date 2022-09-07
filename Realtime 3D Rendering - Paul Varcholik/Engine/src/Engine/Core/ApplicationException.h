#pragma once

#include <exception>

namespace Engine
{
	class ApplicationException : public std::exception
	{
	public:
		ApplicationException() = default;
		ApplicationException(char const* const _Message, int num) : std::exception(_Message, num) {}
		ApplicationException(char const* const _Message) : std::exception(_Message) {}

		const char* what() { return "Application Exception"; }
	};
}