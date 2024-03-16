#pragma once
#include "../httpRequestAndConfig/.hpp"
#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(const Connection &socket);
