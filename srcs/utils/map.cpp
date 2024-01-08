#include <map>

namespace map
{
template <typename Key, typename Value> const Value value(const std::map<Key, Value> myMap, const Key key)
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
} // namespace map
