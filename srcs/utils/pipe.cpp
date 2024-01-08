#include "../Result/Result.hpp"
#include <unistd.h>

const Result<const int> pipe()
{
    int fds[2];
    if (pipe(fds) == -1)
    {
        return Err(errno);
    }
    return Ok(pipe);
}
