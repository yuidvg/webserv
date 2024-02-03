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
    const std::vector<std::string> tokens = utils::tokenize(authorization, ' ');
    return tokens.size() > 0 ? tokens[0] : "";
}

char *const *enviromentVariables(const HttpRequest &request, const Socket &socket, const Uri &uri)
{
    std::map<std::string, std::string> env;

    env["AUTH_TYPE"] = authType(request);
    env["CONTENT_LENGTH"] = request.body.size();
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
    int pipefds[2];
    if (pipe(pipefds) == -1)
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
        return (SERVER_ERROR_RESPONSE);
    else if (pid == 0) // child process
    {
        std::cout << "child process" << std::endl;
        close(pipefds[OUT]);
        dup2(pipefds[IN], STDOUT_FILENO);
        close(pipefds[IN]);
        errno = 0;
        execve(uri.scriptPath.c_str(), args, envp);
        exit(errno);
    }
    else // parent process
    {
        close(pipefds[IN]);
        std::string response = "";
        char buffer[1024];

        ssize_t n;
        while ((n = read(pipefds[OUT], buffer, sizeof(buffer))) > 0)
        {
            response.append(buffer, n);
        }
        if (n == -1)
        {
            std::cerr << "read failed" << std::endl;
            return (SERVER_ERROR_RESPONSE);
        }
        close(pipefds[OUT]);

        CgiResponse cgiResponse(response);

        int status;
        waitpid(pid, &status, 0);
        const int exitStatus = WEXITSTATUS(status);
        if (WIFEXITED(status) && exitStatus == 0)
            return (
                HttpResponse(SUCCESS, cgiResponse.getBody(), cgiResponse.getContentType(), cgiResponse.getLocation()));
        else
            return (SERVER_ERROR_RESPONSE);
    }
    return (SUCCESS_RESPONSE);
}
