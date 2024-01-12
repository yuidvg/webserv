#ifndef DIRECTIVE_PARSER_HPP
#define DIRECTIVE_PARSER_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include "parseConfig.hpp"

namespace directiveParser
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
} // namespace directiveParser

directiveParser::ServerResult parseServer(Tokenize token, std::vector<Tokenize> &tokens);
directiveParser::NameResult parseServerName(const Tokenize &token);
directiveParser::PortResult parseListen(const Tokenize &token);
directiveParser::ErrorPagesResult parseErrorPage(const Tokenize &token);
directiveParser::ClientMaxBodySizeResult parseClientMaxBodySize(const Tokenize &token);
directiveParser::StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);
directiveParser::LocationResult parseLocationDirective(Tokenize token, std::vector<Tokenize> &tokens);

directiveParser::RootResult parseRootDirective(const Tokenize &token);
directiveParser::AutoindexResult parseAutoindexDirective(const Tokenize &token);
directiveParser::IndexResult parseIndexDirective(const Tokenize &token);
directiveParser::RedirectResult parseReturnDirective(const Tokenize &token);
directiveParser::AllowMethodsResult parseAllowMethodDirective(const Tokenize &token);
directiveParser::CgiExtensionResult parseCgiExecutorDirective(const Tokenize &token);
directiveParser::UploadPathResult parseUploadPathDirective(const Tokenize &token);
directiveParser::RedirectResult parseReturnDirective(const Tokenize &token);

#endif
