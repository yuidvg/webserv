#pragma once

#include "external.hpp"

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

