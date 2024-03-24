#pragma once
#include "top.hpp"

ParseCgiResponseResult parseCgiResponse(std::string const &response);
CgiRequestOrHttpResponse processCgiResponse(const CgiResponse &cgiResponse, const HttpRequest &request,
                                            const Client &client, const ErrorPagePaths &errorPages);