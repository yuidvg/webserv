#pragma once

template <typename T> class Option
{
  private:
    const bool isSome;
    const T value;

  public:
    Option(const T &value) : isSome(true), value(value)
    {
    }
    Option() : isSome(false), value(T())
    {
    }
    ~Option()
    {
    }
    Option(const Option &other) : isSome(other.isSome), value(other.value)
    {
    }
    Option &operator=(const Option &other)
    {
        value = const_cast<T &>(other.value);
        isSome = const_cast<bool &>(other.isSome);
        return *this;
    }
    operator bool() const
    {
        return isSome;
    }
    T const &operator*() const
    {
        return value;
    }
};
