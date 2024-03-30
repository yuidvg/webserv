#include "../all.hpp"

namespace utils
{
bool isStatusInRange(const int status)
{
    return status >= 100 && status <= 500;
}
} // namespace utils
