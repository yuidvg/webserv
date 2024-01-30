#pragma once
#include "../webserv.hpp"

// script-URI = <scheme> "://" <server-name> ":" <server-port>
//     <script-path> <extra-path> "?" <query-string>
struct ScriptUri
{
    const std::string scriptPath;
    const std::string extraPath;
    const std::string queryString;

    ScriptUri(const std::string &scriptPath, const std::string &extraPath, const std::string &queryString)
        : scriptPath(scriptPath), extraPath(extraPath), queryString(queryString)
    {
    }
    ScriptUri() : scriptPath(), extraPath(), queryString()
    {
    }
};
