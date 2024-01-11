#ifndef UTILS_ALL_HPP
#define UTILS_ALL_HPP

#include "../consts.hpp"
#include "../types/all.hpp"

namespace utils
{
Location matchedLocation(const std::string uri, const std::vector<Location> locations);
unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern);

// string
std::string &trim(std::string &str);
std::string lowerCase(const std::string str);
bool isNumber(const std::string str);
template <typename T> std::string toString(const T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// fd set
fd_set fdSetFrom(const Sockets sockets);
Sockets socketsIn(const fd_set fdSet, const Sockets sockets);

typedef Result<std::string, HttpResponse> FileContentResult;

FileContentResult content(const std::string path);
std::string contentType(const std::string path);
char *toChar(std::string str);

// map
template <typename Key, typename Value> Value value(const std::map<Key, Value> myMap, const Key key)
{
    typename std::map<Key, Value>::const_iterator iterator = myMap.find(key);
    if (iterator != myMap.end())
    {
        return iterator->second;
    }
    else
    {
        return Value();
    }
}

template <typename T> void printError(const T &message)
{
    std::cerr << message << std::endl;
}

template <class T> std::string to_string(const T &value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

// vector
template <typename T> bool contained(const T needle, const std::vector<const T> haystack)
{
    return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
}

template <typename T> std::vector<T> combined(const std::vector<T> a, const std::vector<T> b)
{
    std::vector<T> combined;
    for (typename std::vector<T>::const_iterator it = a.begin(); it != a.end(); ++it)
    {
        combined.push_back(*it);
    }
    for (typename std::vector<T>::const_iterator it = b.begin(); it != b.end(); ++it)
    {
        combined.push_back(*it);
    }
    return combined;
}

template <typename T> std::vector<const T> excluded(const std::vector<const T> haystack, const T needle)
{
    std::vector<const T> excluded;
    for (typename std::vector<const T>::const_iterator it = haystack.begin(); it != haystack.end(); ++it)
    {
        if (*it != needle)
        {
            excluded.push_back(*it);
        }
    }
    return excluded;
}

std::string rooted(const std::string uri, const Location location);
std::string indexed(const std::string uri, const Location location);
std::string concatPath(const std::string pathA, const std::string pathB);

// stat
IsDirectoryResult isDirectory(const std::string path);

// server
Server matchedServer(const std::string host, const Servers servers, const Socket socket);

} // namespace utils

#endif
