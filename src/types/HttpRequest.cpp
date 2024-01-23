#include "HttpRequest.hpp"

HttpRequest::HttpRequest(){};

HttpRequest::HttpRequest(const std::string m, const std::string u, const std::string v, const Headers h,
                         const std::string b)
    : method(m), target(u), version(v), headers(h), body(b){};
