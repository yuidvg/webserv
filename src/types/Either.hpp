#pragma once

template <typename L, typename R> struct Either
{
    const L leftValue;
    const R rightValue;
    const bool success;

    Either(const L leftValue, const R rightValue, const bool success)
        : leftValue(leftValue), rightValue(rightValue), success(success){};

    static Either<L, R> Right(const R rightValue)
    {
        return (Either<L, R>(L(), rightValue, true));
    };
    static Either<L, R> Left(const L leftValue)
    {
        return (Either<L, R>(rightValue, R(), false));
    };
};
