#include "../types/all.hpp"

namespace utils
{
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

template <typename T> bool contains(const T needle, const std::vector<const T> haystack)
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

template <typename T> unsigned int max(const std::vector<T> list)
{
    unsigned int maxSd = 0;
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (static_cast<int>(list[i]) > static_cast<int>(maxSd))
        {
            maxSd = list[i];
        }
    }
    return maxSd;
}

template <typename T> std::string toString(const T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
} // namespace utils
