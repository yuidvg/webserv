#pragma once
#include "../httpRequestAndConfig/.hpp"
#include "../types/HttpRequestText.hpp"
#include "../webserv.hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText, const Server &server);


bool isLineTooLong(const std::string &line);
bool decodeTarget(std::string &target);
int getRequestLineStatusCode(const RequestLine requestLine);
std::string getlineCustom(std::istringstream &requestTextStream);
bool decodeTarget(std::string &target);
GetRequestLineResult getRequestLine(std::istringstream &requestTextStream);
