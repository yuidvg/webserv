#pragma once

#include "external.hpp"

enum TernaryTag
{
    TERNARY_FIRST,
    TERNARY_SECOND,
    TERNARY_THIRD
};

template <typename F, typename S, typename T> struct Ternary
{
    const TernaryTag tag;
    const F first;
    const S second;
    const T third;

    // Canonnical
    Ternary() : tag(TERNARY_FIRST), first(F()), second(S()), third(T())
    {
    }
    ~Ternary()
    {
    }
    Ternary(const Ternary<F, S, T> &ternary)
        : tag(ternary.tag), first(ternary.first), second(ternary.second), third(ternary.third)
    {
    }
    Ternary &operator=(const Ternary<F, S, T> &ternary)
    {
        return Ternary(ternary);
    }

    Ternary(const TernaryTag tag, const F first, const S second, const T third)
        : tag(tag), first(first), second(second), third(third)
    {
    }

    static Ternary<F, S, T> First(const F first)
    {
        return Ternary<F, S, T>(TERNARY_FIRST, first, S(), T());
    }
    static Ternary<F, S, T> Second(const S second)
    {
        return Ternary<F, S, T>(TERNARY_SECOND, F(), second, T());
    }
    static Ternary<F, S, T> Third(const T third)
    {
        return Ternary<F, S, T>(TERNARY_THIRD, F(), S(), third);
    }
};
