#include "../types/bottom.hpp"

namespace utils
{
template <typename Key, typename Value> Value value(const std::map<Key, Value> myMap, Key key)
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

template <typename T> void printError(T &message)
{
    std::cerr << message << std::endl;
}

template <typename T> bool contains(const T &needle, const std::vector<T> &haystack)
{
    return std::find(haystack.begin(), haystack.end(), needle) != haystack.end();
}

template <typename T> std::vector<T> concat(const std::vector<T> &a, const std::vector<T> &b)
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

template <typename T> std::vector<T> exclude(const std::vector<T> haystack, const T needle)
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

template <typename T> std::vector<T> exclude(const std::vector<T> haystack, const std::vector<T> needles)
{
    std::vector<T> excluded;
    for (typename std::vector<T>::const_iterator it = haystack.begin(); it != haystack.end(); ++it)
    {
        if (!contains(*it, needles))
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

template <typename T> void appendVector(std::vector<T> &destination, const std::vector<T> &source)
{
    for (typename std::vector<T>::const_iterator it = source.begin(); it != source.end(); ++it)
    {
        destination.push_back(*it);
    }
}

template <typename Container, typename F, typename R> std::vector<R> map(const Container &cont, F f)
{
    std::vector<R> result;
    for (typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it)
    {
        result.push_back(f(*it));
    }
    return result;
}

template <typename Container, typename F, typename R, typename A> std::vector<R> map(const Container &cont, F f, A arg)
{
    std::vector<R> result;
    for (typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it)
    {
        result.push_back(f(*it, arg));
    }
    return result;
}

} // namespace utils
