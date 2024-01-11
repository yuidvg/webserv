#ifndef TOKENIZE_CONFIG_HPP
#define TOKENIZE_CONFIG_HPP

#include "parseConfig.hpp"

#define SERVER "server"
#define SERVER_NAME "server_name"
#define LISTEN "listen"
#define LOCATION "location"
#define ALLOW_METHOD "allow_method"
#define CGI_EXECUTOR "cgi_executor"
#define UPLOAD_PATH "upload_path"
#define RETURN "return"
#define ROOT "root"
#define ERROR_PAGE "error_page"
#define CLIENT_MAX_BODY_SIZE "client_max_body_size"
#define INDEX "index"
#define AUTOINDEX "autoindex"
#define OPEN_BRACKET "{"
#define CLOSE_BRACKET "}"

struct Tokenize
{
    std::string key;
    std::vector<std::string> values;
};

typedef utils::Result<std::vector<Tokenize>, std::string> tokenizeResult;
typedef utils::Result<std::string, std::string> stringResult;

// stringResult checkBracketsBalance(const std::vector<Tokenize> &tokens);
stringResult removeSemicolonFromToken(Tokenize &token);
tokenizeResult tokenize(const char *configPath);

#endif
