#include ".hpp"
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

char *const *enviromentVariables(const HttpRequest &request, const Socket &socket, const ScriptUri &scriptUri)
{
    std::map<std::string, std::string> env;

    env["AUTH_TYPE"] = authType(request);
    env["CONTENT_LENGTH"] = request.body.size();
    env["CONTENT_TYPE"] = utils::value(request.headers, std::string("Content-Type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = scriptUri.extraPath;
    env["PATH_TRANSLATED"] =
        comply(scriptUri.extraPath, CONFIG.getServer(request.host, socket.port).getLocation(request.target));
    env["QUERY_STRING"] = scriptUri.queryString;
    env["REMOTE_ADDR"] = socket.opponentIp;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = scriptUri.scriptPath;
    env["SERVER_NAME"] = request.host;
    env["SERVER_PORT"] = socket.port;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    return mapStringStringToCStringArray(env);
}
} // namespace

HttpResponse executeCgi(const HttpRequest &request, const Socket &socket, const ScriptUri &scriptUri)
{
    int pipefds[2];
    if (pipe(pipefds) == -1)
    {
        std::cerr << "pipe failed" << std::endl;
        return (SERVER_ERROR_RESPONSE);
    }
    const pid_t pid = fork();
    if (pid == -1)
        return (SERVER_ERROR_RESPONSE);
    else if (pid == 0) // child process
    {
        close(pipefds[IN]);
        char *args[2];
        args[0] = const_cast<char *>(request.target.c_str());
        args[1] = NULL;
        execve(request.target.c_str(), args, enviromentVariables(request, socket, scriptUri));
        std::cerr << "execve failed" << std::endl;
    }
    else // parent process
    {
        close(pipefds[OUT]);
        write(pipefds[IN], request.body.c_str(), request.body.size());
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            return (SUCCESS_RESPONSE);
        else
            return (SERVER_ERROR_RESPONSE);
    }
    return (SUCCESS_RESPONSE);
}
