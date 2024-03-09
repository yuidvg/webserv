#pragma once

#include "external.hpp"

template <typename T, typename E> struct Result
{
    const T value;
    const E error;
    const bool success;

    Result(const T value, const E error, const bool success) : value(value), error(error), success(success){};

    static Result<T, E> Success(const T value)
    {
        return (Result<T, E>(value, E(), true));
    };
    static Result<T, E> Error(const E error)
    {
        return (Result<T, E>(T(), error, false));
    };
};
