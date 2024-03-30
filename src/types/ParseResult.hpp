#pragma once

#include "external.hpp"

enum ParseStatus
{
    PENDING,
    PARSED,
    ERROR
};

template <typename T, typename E> struct ParseResult
{
    ParseStatus status;
    const T value;
    const E error;

    ParseResult() : status(PENDING), value(T()), error(E()){};
    ParseResult(const T value, const E error, const ParseStatus status) : status(status), value(value), error(error){};

    static ParseResult<T, E> Success(const T value)
    {
        return (ParseResult<T, E>(value, E(), PARSED));
    };
    static ParseResult<T, E> Error(const E error)
    {
        return (ParseResult<T, E>(T(), error, ERROR));
    };
    static ParseResult<T, E> Pending()
    {
        return (ParseResult<T, E>());
    };
};
