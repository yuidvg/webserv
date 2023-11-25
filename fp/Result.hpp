#ifndef RESULT_HPP
#define RESULT_HPP

#include <cassert>
#include <string>

struct Error
{
	const std::string message;
	Error(const std::string initialMessage = "") : message(initialMessage)
	{
	}
};

template <typename T> struct Result
{
	const T value;
	const Error error;
	const bool ok;

	Result(const T initialValue) : value(initialValue), error(), ok(true)
	{
	}
	Result(const Error err) : value(), error(err), ok(false)
	{
	}
};

#endif
