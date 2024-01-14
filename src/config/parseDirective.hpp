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
parseDirective::StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);

parseDirective::ServerResult parseServer(const std::vector<std::string> directiveTokens,
                                         std::vector<std::string> &tokens);
parseDirective::NameResult parseServerName(const std::vector<std::string> directiveTokens);
parseDirective::PortResult parseListen(const std::vector<std::string> directiveTokens);
parseDirective::ErrorPagesResult parseErrorPage(const std::vector<std::string> directiveTokens);
parseDirective::ClientMaxBodySizeResult parseClientMaxBodySize(const std::vector<std::string> directiveTokens);
parseDirective::LocationResult parseLocationDirective(const std::vector<std::string> directiveTokens,
                                                      std::vector<std::string> &tokens);

parseDirective::RootResult parseRootDirective(const std::vector<std::string> directiveTokens);
parseDirective::AutoindexResult parseAutoindexDirective(const std::vector<std::string> directiveTokens);
parseDirective::IndexResult parseIndexDirective(const std::vector<std::string> directiveTokens);
parseDirective::RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens);
parseDirective::AllowMethodsResult parseAllowMethodDirective(const std::vector<std::string> directiveTokens);
parseDirective::CgiExtensionResult parseCgiExecutorDirective(const std::vector<std::string> directiveTokens);
parseDirective::UploadPathResult parseUploadPathDirective(const std::vector<std::string> directiveTokens);
parseDirective::RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens);

#endif
