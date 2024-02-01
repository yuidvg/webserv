#pragma once
#include "external.hpp"

// script-URI = <scheme> "://" <server-name> ":" <server-port>
//     <script-path> <extra-path> "?" <query-string>
struct Uri
{
    const std::string scriptPath;
    const std::string extraPath;
    const std::string queryString;

    Uri(const std::string &scriptPath, const std::string &extraPath, const std::string &queryString)
        : scriptPath(scriptPath), extraPath(extraPath), queryString(queryString)
    {
    }
    Uri() : scriptPath(), extraPath(), queryString()
    {
    }
};
