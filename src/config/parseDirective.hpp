#pragma once

#include "../types/.hpp"

namespace parseConfig
{
LocationResult parseLocationDirective(const std::vector<std::string> directiveTokens, std::vector<std::string> &tokens);
ServerResult parseServer(const std::vector<std::string> directiveTokens, std::vector<std::string> &tokens);
NameResult parseServerName(const std::vector<std::string> directiveTokens);
PortResult parseListen(const std::vector<std::string> directiveTokens);
ErrorPageResult parseErrorPage(const std::vector<std::string> directiveTokens);
ClientMaxBodySizeResult parseClientMaxBodySize(const std::vector<std::string> directiveTokens);
RootResult parseRootDirective(const std::vector<std::string> directiveTokens);
AutoindexResult parseAutoindexDirective(const std::vector<std::string> directiveTokens);
IndexResult parseIndexDirective(const std::vector<std::string> directiveTokens);
RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens);
AllowMethodsResult parseAllowMethodDirective(const std::vector<std::string> directiveTokens);
CgiExtensionResult parseCgiExecutorDirective(const std::vector<std::string> directiveTokens);
UploadPathResult parseUploadPathDirective(const std::vector<std::string> directiveTokens);
RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens);
} // namespace parseConfig
