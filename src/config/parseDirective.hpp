#ifndef PARSE_DIRECTIVE_HPP
#define PARSE_DIRECTIVE_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include "parseConfig.hpp"

namespace parseDirective
{
typedef utils::Result<Server, std::string> ServerResult;
typedef utils::Result<std::string, std::string> NameResult;
typedef utils::Result<int, std::string> PortResult;
typedef utils::Result<Location, std::string> LocationResult;

typedef utils::Result<std::map<int, std::string>, std::string> ErrorPagesResult;
typedef utils::Result<size_t, std::string> ClientMaxBodySizeResult;

typedef utils::Result<std::string, std::string> PathResult;
typedef utils::Result<std::string, std::string> RootResult;
typedef utils::Result<bool, std::string> AutoindexResult;
typedef utils::Result<std::string, std::string> IndexResult;
typedef utils::Result<std::vector<std::string>, std::string> AllowMethodsResult;
typedef utils::Result<std::string, std::string> CgiExtensionResult;
typedef utils::Result<std::string, std::string> UploadPathResult;
typedef utils::Result<std::map<int, std::string>, std::string> RedirectResult;

typedef utils::Result<int, std::string> StringToIntResult;
} // namespace parseDirective

parseDirective::ServerResult parseServer(Tokenize token, std::vector<Tokenize> &tokens);
parseDirective::NameResult parseServerName(const Tokenize &token);
parseDirective::PortResult parseListen(const Tokenize &token);
parseDirective::ErrorPagesResult parseErrorPage(const Tokenize &token);
parseDirective::ClientMaxBodySizeResult parseClientMaxBodySize(const Tokenize &token);
parseDirective::StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);
parseDirective::LocationResult parseLocationDirective(Tokenize token, std::vector<Tokenize> &tokens);

parseDirective::RootResult parseRootDirective(const Tokenize &token);
parseDirective::AutoindexResult parseAutoindexDirective(const Tokenize &token);
parseDirective::IndexResult parseIndexDirective(const Tokenize &token);
parseDirective::RedirectResult parseReturnDirective(const Tokenize &token);
parseDirective::AllowMethodsResult parseAllowMethodDirective(const Tokenize &token);
parseDirective::CgiExtensionResult parseCgiExecutorDirective(const Tokenize &token);
parseDirective::UploadPathResult parseUploadPathDirective(const Tokenize &token);
parseDirective::RedirectResult parseReturnDirective(const Tokenize &token);

#endif
