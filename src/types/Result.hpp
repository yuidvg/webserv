#pragma once

#include "external.hpp"

template <typename T, typename E> struct Result
{
    T value;
    E error;
    const bool success;

    Result(T &value, E &error, bool success) : value(value), error(error), success(success){};

    static Result<T, E> Success(T &value)
    {
        return (Result<T, E>(value, E(), true));
    };
    static Result<T, E> Error(E &error)
    {
        return (Result<T, E>(T(), error, false));
    };
};
