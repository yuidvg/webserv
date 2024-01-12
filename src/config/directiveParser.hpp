#ifndef DIRECTIVE_PARSER_HPP
#define DIRECTIVE_PARSER_HPP

#include "../utils/utils.hpp"
#include "config.hpp"
#include "parseConfig.hpp"

namespace directiveParser
{

// server専用
typedef utils::Result<Server, std::string> serverResult;
typedef utils::Result<std::string, std::string> nameResult;
typedef utils::Result<int, std::string> portResult;
typedef utils::Result<Location, std::string> locationResult;
// server, location両方
typedef utils::Result<std::map<int, std::string>, std::string> errorPagesResult;
typedef utils::Result<size_t, std::string> clientMaxBodySizeResult;
// location専用
typedef utils::Result<std::string, std::string> pathResult;
typedef utils::Result<std::string, std::string> rootResult;
typedef utils::Result<bool, std::string> autoindexResult;
typedef utils::Result<std::string, std::string> indexResult;
typedef utils::Result<std::vector<std::string>, std::string> allowMethodsResult;
typedef utils::Result<std::string, std::string> cgiExtensionResult;
typedef utils::Result<std::string, std::string> uploadPathResult;
typedef utils::Result<std::map<int, std::string>, std::string> redirectResult;
// その他
typedef utils::Result<int, std::string> stringToIntResult;
} // namespace directiveParser

directiveParser::serverResult parseServer(Tokenize token, std::vector<Tokenize> &tokens);
directiveParser::nameResult parseServerName(const Tokenize &token);
directiveParser::portResult parseListen(const Tokenize &token);
directiveParser::errorPagesResult parseErrorPage(const Tokenize &token);
directiveParser::clientMaxBodySizeResult parseClientMaxBodySize(const Tokenize &token);
directiveParser::stringToIntResult stringToInt(const std::string &str, int minVal, int maxVal);
directiveParser::locationResult parseLocationDirective(Tokenize token, std::vector<Tokenize> &tokens);
// location
directiveParser::rootResult parseRootDirective(const Tokenize &token);
directiveParser::autoindexResult parseAutoindexDirective(const Tokenize &token);
directiveParser::indexResult parseIndexDirective(const Tokenize &token);
directiveParser::redirectResult parseReturnDirective(const Tokenize &token);
directiveParser::allowMethodsResult parseAllowMethodDirective(const Tokenize &token);
directiveParser::cgiExtensionResult parseCgiExecutorDirective(const Tokenize &token);
directiveParser::uploadPathResult parseUploadPathDirective(const Tokenize &token);
directiveParser::redirectResult parseReturnDirective(const Tokenize &token);

#endif
