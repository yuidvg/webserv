#include "Result.hpp"

template <typename T> Result<T> tryCatch(const T (*const f)())
{
	try
	{
		return f();
	}
	catch (const Error err)
	{
		return err;
	}
}