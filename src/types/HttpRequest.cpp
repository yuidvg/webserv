#include "HttpRequest.hpp"

HttpRequest::HttpRequest(){};

HttpRequest::HttpRequest(const std::string &m, const std::string &t, const std::string &ho, const std::string &v,
                         const Headers &h, const std::string &b)
    : method(m), target(t), host(ho), version(v), headers(h), body(b){};
