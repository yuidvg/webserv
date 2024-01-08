#include "utils.hpp"

template <typename T>
void printError(const T &message)
{
    std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
}
