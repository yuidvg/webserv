#ifndef PARSE_DIRECTIVE_HPP
#define PARSE_DIRECTIVE_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include "parseConfig.hpp"

namespace parseDirective
{
typedef Result<Server, std::string> ServerResult;
typedef Result<std::string, std::string> NameResult;
typedef Result<int, std::string> PortResult;
typedef Result<Location, std::string> LocationResult;

typedef Result<std::map<int, std::string>, std::string> ErrorPagesResult;
typedef Result<size_t, std::string> ClientMaxBodySizeResult;

typedef Result<std::string, std::string> PathResult;
typedef Result<std::string, std::string> RootResult;
typedef Result<bool, std::string> AutoindexResult;
typedef Result<std::string, std::string> IndexResult;
typedef Result<std::vector<std::string>, std::string> AllowMethodsResult;
typedef Result<std::string, std::string> CgiExtensionResult;
typedef Result<std::string, std::string> UploadPathResult;
typedef Result<std::map<int, std::string>, std::string> RedirectResult;

typedef Result<int, std::string> StringToIntResult;
} // namespace parseDirective
parseDirective::LocationResult parseLocationDirective(const std::vector<std::string> directiveTokens,
                                                      std::vector<std::string> &tokens);
parseDirective::ServerResult parseServer(const std::vector<std::string> directiveTokens,
                                         std::vector<std::string> &tokens);
parseDirective::StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);

parseDirective::NameResult parseServerName(const std::vector<std::string> directiveTokens);
parseDirective::PortResult parseListen(const std::vector<std::string> directiveTokens);
parseDirective::ErrorPagesResult parseErrorPage(const std::vector<std::string> directiveTokens);
parseDirective::ClientMaxBodySizeResult parseClientMaxBodySize(const std::vector<std::string> directiveTokens);

parseDirective::RootResult parseRootDirective(const std::vector<std::string> directiveTokens);
parseDirective::AutoindexResult parseAutoindexDirective(const std::vector<std::string> directiveTokens);
parseDirective::IndexResult parseIndexDirective(const std::vector<std::string> directiveTokens);
parseDirective::RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens);
parseDirective::AllowMethodsResult parseAllowMethodDirective(const std::vector<std::string> directiveTokens);
parseDirective::CgiExtensionResult parseCgiExecutorDirective(const std::vector<std::string> directiveTokens);
parseDirective::UploadPathResult parseUploadPathDirective(const std::vector<std::string> directiveTokens);
parseDirective::RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens);

#endif
