#include "HttpRequest.hpp"

HttpRequest::HttpRequest(){};

HttpRequest::HttpRequest(const std::string &m, const std::string &t, const std::string &v, const Headers &h,
                         const std::string &b, const std::string &host)
    : method(m), target(t), version(v), headers(h), body(b), host(host){};
