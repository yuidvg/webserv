#pragma once

template <typename T> struct Option
{
    const bool isSome;
    T value;
    Option() : isSome(false), value(T())
    {
    }
    Option(T &value) : isSome(true), value(value)
    {
    }
};
