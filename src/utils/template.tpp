#include "../types/.hpp"

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

template <typename T> bool contains(const T needle, const std::vector<T> haystack)
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

template <typename T> std::vector<T> excluded(const std::vector<T> haystack, const T needle)
{
    std::vector<T> excluded;
    for (typename std::vector<T>::const_iterator it = haystack.begin(); it != haystack.end(); ++it)
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
    unsigned int maxSocket = 0;
    for (size_t i = 0; i < list.size(); ++i)
    {
        if (static_cast<int>(list[i]) > static_cast<int>(maxSocket))
        {
            maxSocket = list[i];
        }
    }
    return maxSocket;
}

template <typename T> std::string toString(const T value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template <typename T, typename Predicate> std::vector<T> filter(const std::vector<T> &vec, Predicate pred)
{
    std::vector<T> result;
    for (typename std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (pred(*it))
        {
            result.push_back(*it);
        }
    }
    return result;
}

} // namespace utils
