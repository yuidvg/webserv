#pragma once

#include "external.hpp"

template <typename T, typename E> struct HttpRequestParseResult
{
    HttpRequestParseStatus status;
    const T value;
    const E error;

    HttpRequestParseResult() : status(PENDING){};
    HttpRequestParseResult(const T value, const E error, const ParseStatus status)
        : value(value), error(error), status(status){};

    static HttpRequestParseResult<T, E> Success(const T value)
    {
        return (HttpRequestParseResult<T, E>(value, E(), SUCCESS));
    };
    static HttpRequestParseResult<T, E> Error(const E error)
    {
        return (HttpRequestParseResult<T, E>(T(), error, ERROR));
    };
    static HttpRequestParseResult<T, E> Pending()
    {
        return (HttpRequestParseResult<T, E>());
    };
};
