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

parseDirective::ServerResult parseServer(Token token, std::vector<Token> &tokens);
parseDirective::NameResult parseServerName(const Token &token);
parseDirective::PortResult parseListen(const Token &token);
parseDirective::ErrorPagesResult parseErrorPage(const Token &token);
parseDirective::ClientMaxBodySizeResult parseClientMaxBodySize(const Token &token);
parseDirective::StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);
parseDirective::LocationResult parseLocationDirective(Token token, std::vector<Token> &tokens);

parseDirective::RootResult parseRootDirective(const Token &token);
parseDirective::AutoindexResult parseAutoindexDirective(const Token &token);
parseDirective::IndexResult parseIndexDirective(const Token &token);
parseDirective::RedirectResult parseReturnDirective(const Token &token);
parseDirective::AllowMethodsResult parseAllowMethodDirective(const Token &token);
parseDirective::CgiExtensionResult parseCgiExecutorDirective(const Token &token);
parseDirective::UploadPathResult parseUploadPathDirective(const Token &token);
parseDirective::RedirectResult parseReturnDirective(const Token &token);

#endif
