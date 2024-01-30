#pragma once
#include "../webserv.hpp"

HttpResponse executeCgi(const HttpRequest &request, const Socket &socket, const ScriptUri &scriptUri);
bool findCgiExtension(const std::string &target, const std::string &extension);

ScriptUri retrieveScriptUri(const std::string &target, const std::string &cgiExtension);