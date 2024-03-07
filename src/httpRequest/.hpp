#pragma once
#include "../httpRequestAndConfig/.hpp"
#include "../types/HttpRequestText.hpp"
#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(const std::string message);

bool isHttpMessage(const std::string &message);
bool isLineTooLong(const std::string &line);
int getRequestLineStatusCode(const RequestLine requestLine);
bool validateAndDecodeTarget(std::string &target);
GetRequestLineResult getRequestLine(std::istringstream &requestTextStream);
