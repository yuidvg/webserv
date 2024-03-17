#pragma once
#include "../webserv.hpp"

ParseCgiResponseResult parseCgiResponse(std::string const &response);
ImmidiateResponse processCgiResponse(const CgiResponse &cgiResponse, const HttpRequest &request, const Client &client,
                                     const ErrorPages &errorPages);