#pragma once

#include "external.hpp"

template <typename T, typename E> struct ParseResult
{
    ParseStatus status;
    const T value;
    const E error;

    ParseResult() : status(PENDING){};
    ParseResult(const T value, const E error, const ParseStatus status)
        : value(value), error(error), status(status){};

    static ParseResult<T, E> Success(const T value)
    {
        return (ParseResult<T, E>(value, E(), SUCCESS));
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
