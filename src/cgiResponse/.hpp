#pragma once
#include "../webserv.hpp"

ParseCgiResponseResult parseCgiResponse(const std::string &text);

ParseCgiResponseResult parseCgiDocumentResponse(const std::string &text);
ParseCgiResponseResult parseCgiLocalRedirectResponse(const std::string &text);
ParseCgiResponseResult parseCgiClientRedirectResponse(const std::string &text);
ParseCgiResponseResult parseCgiClientRedirectWithDocumentResponse(const std::string &text);

