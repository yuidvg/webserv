#pragma once

#include "../config/parseConfig.hpp"
#include "../config/parseDirective.hpp"
#include "../config/tokenizeConfig.hpp"

Server getServer(const HttpRequest &httpRequest);
Location getLocation(const HttpRequest &httpRequest);
HttpResponse getErrorResponse(const HttpRequest &httpRequest, const int statusCode);