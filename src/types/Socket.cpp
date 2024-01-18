#include "Socket.hpp"

Socket::Socket() : descriptor(-1), server("", 80, std::map<int, std::string>(), 1048576, std::vector<Location>())
{
}

Socket::Socket(const unsigned int descriptor, const Server server) : descriptor(descriptor), server(server)
{
}

bool Socket::operator==(const Socket other) const
{
    return this->descriptor == other.descriptor;
};

bool Socket::operator!=(const Socket other) const
{
    return this->descriptor != other.descriptor;
};
