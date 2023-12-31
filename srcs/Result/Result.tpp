#ifndef RESULT_TPP
# define RESULT_TPP

# include "Result.hpp"

template <typename T, typename E>
Result<T, E>::Result() : _value(NULL), _is_ok(false) {}

template <typename T, typename E>
Result<T, E>::~Result()
{
	if (_value)
		delete _value;
}

template <typename T, typename E>
Result<T, E>	Result<T, E>::Ok(const T &value)
{
	Result<T, E>	result;
	result._value = new T(value);
	result._is_ok = true;
	return (result);
}

template <typename T, typename E>
Result<T, E>	Result<T, E>::Err(E error)
{
	Result<T, E>	result;
	result._error = error;
	result._is_ok = false;
	return (result);
}

template <typename T, typename E>
bool	Result<T, E>::ok() const
{
	return (_is_ok);
}

template <typename T, typename E>
T	Result<T, E>::unwrap()
{
	if (!_is_ok)
		throw std::runtime_error("Attempted to unwrap an error");
	return (*_value);
}

template <typename T, typename E>
E	Result<T, E>::unwrapErr()
{
	if (_is_ok)
		throw std::runtime_error("Attempted to unwrap a value as an error");
	return (_error);
}

#endif
