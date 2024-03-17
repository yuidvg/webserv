#pragma once
#include "Cgi.hpp"
#include "external.hpp"

namespace utils
{
bool registerEvent(const uintptr_t identifier, const int16_t filter);
} // namespace utils
class Cgis
{
  private:
    std::vector<Cgi> _cgis;

  public:
    Cgis() : _cgis(){};
    ~Cgis(){};
    Cgi &operator[](const int sd)
    {
        for (std::vector<Cgi>::iterator it = _cgis.begin(); it != _cgis.end(); ++it)
        {
            if (it->sd == sd)
            {
                return *it;
            }
        }
        return *_cgis.begin();
    };
    bool has(const int sd) const
    {
        for (std::vector<Cgi>::const_iterator it = _cgis.begin(); it != _cgis.end(); ++it)
        {
            if (it->sd == sd)
            {
                return true;
            }
        }
        return false;
    };
    void operator+=(const Cgi &cgi)
    {
        _cgis.push_back(cgi);
    };
    void operator+=(const Cgis &otherCgis)
    {
        for (std::vector<Cgi>::const_iterator it = otherCgis._cgis.begin(); it != otherCgis._cgis.end(); ++it)
        {
            _cgis.push_back(*it);
            utils::registerEvent(it->sd, EVFILT_READ);
            utils::registerEvent(it->sd, EVFILT_WRITE);
        }
    };
    void operator-=(const Cgi &cgi)
    {
        for (std::vector<Cgi>::iterator it = _cgis.begin(); it != _cgis.end(); ++it)
        {
            if (*it == cgi)
            {
                _cgis.erase(it);
                return;
            }
        }
    };
};
