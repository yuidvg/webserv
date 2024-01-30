#include ".hpp"
#include "../httpRequestAndConfig/.hpp"

namespace
{
char *const *mapStringStringToCStringArray(const std::map<std::string, std::string> envMap)
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
    const std::vector<const std::string> tokens = utils::tokenize(authorization, ' ');
    return tokens.size() > 0 ? tokens[0] : "";
}

std::string::size_type findPathInfoPos(const std::string &target)
{
    const std::string::size_type firstDotPos = target.find(".");
    return target.find("/", firstDotPos);
}

char *const *enviromentVariables(const HttpRequest &request, const Socket &socket)
{
    std::map<std::string, std::string> env;
    const std::string::size_type pathInfoPos = findPathInfoPos(request.target);
    const std::string pathInfo = request.target.substr(pathInfoPos);
    const std::string scriptName = request.target.substr(0, pathInfoPos);

    env["AUTH_TYPE"] = authType(request);
    env["CONTENT_LENGTH"] = request.body.size();
    env["CONTENT_TYPE"] = utils::value(request.headers, std::string("Content-Type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = pathInfo;
    env["PATH_TRANSLATED"] = comply(pathInfo, CONFIG.getServer(request.host, socket.port).getLocation(request.target));
    env["QUERY_STRING"] = request.target.substr(request.target.find("?") + 1);
    env["REMOTE_ADDR"] = socket.opponentIp;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = scriptName;
    env["SERVER_NAME"] = request.host;
    env["SERVER_PORT"] = socket.port;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    return mapStringStringToCStringArray(env);
}
} // namespace

ResponseResult execute(const HttpRequest &request, const Socket &socket)
{
    int pipefds[2];
    if (pipe(pipefds) == -1)
    {
        std::cerr << "pipe failed" << std::endl;
        return ResponseResult::Error(SERVER_ERROR_RESPONSE);
    }
    const pid_t pid = fork();
    if (pid == -1)
        return ResponseResult::Error(SERVER_ERROR_RESPONSE);
    else if (pid == 0) // child process
    {
        close(pipefds[IN]);
        execve(request.target.c_str(), NULL, enviromentVariables(request, socket));
        std::cerr << "execve failed" << std::endl;
    }
    else // parent process
    {
        close(pipefds[OUT]);
        write(pipefds[IN], request.body.c_str(), request.body.size());
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            return ResponseResult::Error(SERVER_ERROR_RESPONSE);
        else
            return ResponseResult::Error(SERVER_ERROR_RESPONSE);
    }
    return ResponseResult::Success(SUCCESS_RESPONSE);
}
