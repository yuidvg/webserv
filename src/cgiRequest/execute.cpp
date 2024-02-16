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
    const std::string authorization = utils::value(request.headers, std::string("Authorization"));
    const std::vector<std::string> tokens = utils::split(authorization, " ");
    return tokens.size() > 0 ? tokens[0] : "";
}

char *const *enviromentVariables(const HttpRequest &request, const Socket &socket, const Uri &uri)
{
    std::map<std::string, std::string> env;

    env["AUTH_TYPE"] = authType(request);
    env["CONTENT_LENGTH"] = utils::itoa(request.body.size());
    env["CONTENT_TYPE"] = utils::value(request.headers, std::string("Content-Type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = uri.extraPath;
    env["PATH_TRANSLATED"] =
        uri.extraPath.size() > 0
            ? resolvePath(uri.extraPath, CONFIG.getServer(request.host, socket.port).getLocation(request.target))
            : "";
    env["QUERY_STRING"] = uri.queryString;
    env["REMOTE_ADDR"] = socket.opponentIp;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = uri.scriptPath;
    env["SERVER_NAME"] = request.host;
    env["SERVER_PORT"] = socket.port;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    return mapStringStringToCStringArray(env);
}
} // namespace

HttpResponse executeCgi(const HttpRequest &request, const Socket &socket, const Uri &uri)
{
    int requestPipe[2];
    int responsePipe[2];
    if (pipe(requestPipe) == -1 || pipe(responsePipe) == -1)
    {
        std::cerr << "pipe failed" << std::endl;
        return (SERVER_ERROR_RESPONSE);
    }
    char *const *envp = enviromentVariables(request, socket, uri);
    char *args[2];
    args[0] = const_cast<char *>(request.target.c_str());
    args[1] = NULL;
    const pid_t pid = fork();
    if (pid == -1)
    {
        close(requestPipe[OUT]);
        close(requestPipe[IN]);
        close(responsePipe[OUT]);
        close(responsePipe[IN]);
        return (SERVER_ERROR_RESPONSE);
    }
    else if (pid == 0) // child process
    {
        std::cout << "child process" << std::endl;
        close(requestPipe[IN]);
        dup2(requestPipe[OUT], STDIN_FILENO);

        close(responsePipe[OUT]);
        dup2(responsePipe[IN], STDOUT_FILENO);
        errno = 0;
        std::cerr << "execve: " << uri.scriptPath << std::endl;
        execve(uri.scriptPath.c_str(), args, envp);
        std::cerr << "execve failed: " << strerror(errno) << std::endl;
        return (SERVER_ERROR_RESPONSE);
    }
    else // parent process
    {
        close(requestPipe[OUT]);
        close(responsePipe[IN]);
        if (request.body.size() == 0 || write(requestPipe[IN], request.body.c_str(), request.body.size()) > 0)
        {
            close(requestPipe[IN]);
            int status;
            waitpid(pid, &status, 0);
            ReadFileResult readFileResult = utils::readFile(responsePipe[OUT]);
            if (readFileResult.success)
            {
                close(responsePipe[OUT]);
                std::string response = readFileResult.value;

                const ParseCgiResponseResult parseCgiResponseResult = parseCgiResponse(response);
                if (parseCgiResponseResult.success)
                {
                    CgiResponse cgiResponse = parseCgiResponseResult.value;
                    return processCgiResponse(cgiResponse, request, socket);
                }
                else
                {
                    return (parseCgiResponseResult.error);
                }
            }
            else
            {
                std::cerr << "read failed" << std::endl;
                return (SERVER_ERROR_RESPONSE);
            }
        }
        else
        {
            std::cerr << "write failed" << std::endl;
            return (SERVER_ERROR_RESPONSE);
        }
    }
}
