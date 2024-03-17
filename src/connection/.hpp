#pragma once
#include "../webserv.hpp"

void eventLoop(Sockets listenSds);
ImmidiateResponse retrieveImmidiateResponse(const HttpRequest &request, const Connection &connection);