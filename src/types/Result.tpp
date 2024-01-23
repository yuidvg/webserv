
#ifndef RESULT_TPP
#define RESULT_TPP

#include "Result.hpp"

template <typename T, typename E>
Result<T, E>::Result(const T value, const E error, const bool success) : value(value), error(error), success(success)
{
}

template <typename T, typename E> Result<T, E> Result<T, E>::Success(const T value)
{
    return (Result<T, E>(value, E(), true));
}

template <typename T, typename E> Result<T, E> Result<T, E>::Error(const E error)
{
    return (Result<T, E>(T(), error, false));
}

#endif
