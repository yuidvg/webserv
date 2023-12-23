#ifndef RESULT_HPP
#define RESULT_HPP

#include <string>

class Error
{
  private:
    bool isError;
    std::string message;

  public:
    explicit Error(bool isError = true);
    Error(const std::string message);
    ~Error();

    bool isError() const;
    std::string getMessage() const;
}

template <typename T>
class Result
{
  private:
    T value;
    Error error;

  public:
    Result(T value);
    Result(Error error);
    ~Result();

    bool isError() const;
    T getValue() const;
    Error getError() const;
}

#endif
