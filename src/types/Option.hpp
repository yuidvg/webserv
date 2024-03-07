#pragma once

#include <stdexcept> // std::runtime_error

template <typename T> class Option
{
    bool _hasValue;
    T _value;

  public:
    Option() : _hasValue(false), _value(T())
    {
    }

    static Option<T> Some(const T &value)
    {
        Option<T> option;
        option._hasValue = true;
        option._value = value;
        return option;
    }

    static Option<T> None()
    {
        return Option<T>();
    }

    bool hasValue() const
    {
        return _hasValue;
    }

    T getValue() const
    {
        if (!_hasValue)
        {
            throw std::runtime_error("Option does not contain a value");
        }
        return _value;
    }
};
