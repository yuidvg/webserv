#ifndef RESULT_HPP
#define RESULT_HPP

#include <cstring>
#include <stdexcept>

template <typename T, typename E> struct Result
{
    const T value;
    const E error;
    const bool success;

    Result(const T value, const E error, const bool success);
    static Result<T, E> Success(const T value);
    static Result<T, E> Error(const E error);
};

#include "Result.tpp"

#endif