#include ".hpp"
#include "../cgiResponse/.hpp"
#include "../httpRequestAndConfig/.hpp"

namespace
{
char *const *mapStringStringToCStringArray(const std::map<std::string, std::string> &envMap)
{
    char **envArray = new char *[envMap.size() + 1];
    int i = 0;
    for (std::map<std::string, std::string>::const_iterator it = envMap.begin(); it != envMap.end(); ++it)
    {
        std::string envString = it->first + "=" + it->second;
        envArray[i] = new char[envString.size() + 1];
        std::copy(envString.begin(), envString.end(), envArray[i]);
        envArray[i][envString.size()] = '\0'; // Null-terminate the string
        ++i;
    }
    envArray[envMap.size()] = NULL; // Last element is NULL for execve
    return envArray;
}

std::string authType(const HttpRequest &request)
{
    const std::string authorization = utils::value(request.headers, std::string("authorization"));
    const std::vector<std::string> tokens = utils::split(authorization, " ");
    return tokens.size() > 0 ? tokens[0] : "";
}

} // namespace

CgiRequest retrieveCgiRequest(const HttpRequest &request, const Client &connection, const Uri &uri)
{
    std::map<std::string, std::string> env;
    env["AUTH_TYPE"] = authType(request);
    env["CONTENT_LENGTH"] = utils::itoa(request.body.size());
    env["CONTENT_TYPE"] = utils::value(request.headers, std::string("content-type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = uri.extraPath;
    env["PATH_TRANSLATED"] =
        uri.extraPath.size() > 0
            ? resolvePath(uri.extraPath,
                          CONFIG.getServer(request.host, connection.serverPort).getLocation(request.target))
            : "";
    env["QUERY_STRING"] = uri.queryString;
    env["REMOTE_ADDR"] = connection.opponentIp;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = uri.scriptPath;
    env["SERVER_NAME"] = request.host;
    env["SERVER_PORT"] = connection.serverPort;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;

    return CgiRequest(env, uri.scriptPath, request.body);
}

CreateCgiResult createCgi(const CgiRequest &request, const ErrorPages &errorPages, Client &client,
                          const HttpRequest &httpRequest)
{
    int socketPair[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) == 0)
    {
        std::cout << "socketpair succeeded" << std::endl;
        const pid_t pid = fork();
        if (pid == -1)
        {
            close(socketPair[SERVER_END]);
            close(socketPair[CGI]);
            return CreateCgiResult::Error(errorPages.at(SERVER_ERROR));
        }
        else if (pid == 0) // cgi process
        {
            std::cout << "child process" << std::endl;
            close(socketPair[SERVER_END]);
            dup2(socketPair[CGI], STDIN_FILENO);

            errno = 0;
            char *const *envp = mapStringStringToCStringArray(request.envs);
            char *args[2];
            args[0] = const_cast<char *>(request.scriptPath.c_str());
            args[1] = NULL;
            execve(request.scriptPath.c_str(), args, envp);
            std::cerr << "execve failed: " << strerror(errno) << std::endl;
            utils::deleteCStrArray(envp);
            while (true)
            {
                sleep(1);
            }
        }
        else // server process
        {
            close(socketPair[CGI]);
            return CreateCgiResult::Success(Cgi(socketPair[SERVER_END], httpRequest, client));
        }
    }
    else
    {
        std::cerr << "socketpair failed" << std::endl;
        return CreateCgiResult::Error(errorPages.at(SERVER_ERROR));
    }
}
