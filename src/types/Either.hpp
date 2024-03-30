#pragma once

enum EitherTag
{
    LEFT,
    RIGHT
};

template <typename L, typename R> struct Either
{
    L leftValue;
    R rightValue;
    const EitherTag tag;

    Either(const L leftValue, const R rightValue, const EitherTag tag)
        : leftValue(leftValue), rightValue(rightValue), tag(tag){};
    Either(const Either<L, R> &either) : leftValue(either.leftValue), rightValue(either.rightValue), tag(either.tag){};
    Either() : leftValue(L()), rightValue(R()), tag(LEFT){};

    static Either<L, R> Right(const R rightValue)
    {
        return (Either<L, R>(L(), rightValue, RIGHT));
    };
    static Either<L, R> Left(const L leftValue)
    {
        return (Either<L, R>(leftValue, R(), LEFT));
    };
};
