#pragma once
#include "../httpRequestAndConfig/.hpp"
#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(const Socket &socket);
